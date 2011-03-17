#define __PLUGIN__
#define MODULE_STRING "cmplayer-vout"

// gcc -I/Applications/VLC.app/Contents/MacOS/include -I. -L/Applications/VLC.app/Contents/MacOS/lib -std=c99 -lvlc -lvlccore -dynamiclib  myvout_plugin.c -o libcmplayer-vout_plugin.dylib
// from vmem.c of vlc

#include <assert.h>

#include <vlc_common.h>
#include <vlc_plugin.h>
#include <vlc_vout_display.h>
#include <vlc_picture_pool.h>

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/


static int  Open (vlc_object_t *);
static void Close(vlc_object_t *);

vlc_module_begin()
    set_description(MODULE_STRING)
    set_shortname(MODULE_STRING)

    set_category(CAT_VIDEO)
    set_subcategory(SUBCAT_VIDEO_VOUT)
    set_capability("vout display", 0)

    add_string(MODULE_STRING"-chroma", "RV16", NULL, "", "", true)
        change_private()
	add_string(MODULE_STRING "-cb-prepare", "0", NULL, "", "", true)
		change_volatile()
    add_string(MODULE_STRING "-cb-lock", "0", NULL, "", "", true)
        change_volatile()
    add_string(MODULE_STRING "-cb-unlock", "0", NULL, "", "", true)
        change_volatile()
    add_string(MODULE_STRING "-cb-display", "0", NULL, "", "", true)
        change_volatile()
    add_string(MODULE_STRING "-data", "0", NULL, "", "", true)
        change_volatile()
    add_string(MODULE_STRING "-event-handler", "0", NULL, "", "", true)
        change_volatile()
    set_callbacks(Open, Close)
vlc_module_end()

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/
struct picture_sys_t {
    vout_display_sys_t *sys;
    void *id;
};

typedef struct _EventHandler {
	struct _EventHandler *self;
	void (*mousePressed)(struct _EventHandler *self, int button);
	void (*mouseReleased)(struct _EventHandler *self, int button);
	void (*mouseMoved)(struct _EventHandler *self, int x, int y);
	void *vd;
} EventHandler;


static void handleMousePressed(EventHandler *self, int button) {
//	printf("send mouse pressed: %d\n", button); fflush(stdout);
	vout_display_SendEventMousePressed((vout_display_t*)self->vd, button);
}

static void handleMouseReleased(EventHandler *self, int button) {
//	printf("send mouse released: %d\n", button); fflush(stdout);
	vout_display_SendEventMouseReleased((vout_display_t*)self->vd, button);
}

static void handleMouseMoved(EventHandler *self, int x, int y) {
//	printf("send mouse moved: (%d, %d)\n", x, y); fflush(stdout);
	vout_display_SendEventMouseMoved((vout_display_t*)self->vd, x, y);
}

typedef struct _Plane {
		uint8_t *data;
		int dataPitch, dataLines;
		int framePitch, frameLines;
} Plane;

struct vout_display_sys_t {
    picture_pool_t *pool;
    EventHandler *eventHandler;
    void *(*lock)(void *sys, void *planes, int len);
    void (*unlock)(void *sys, void *id, void *const *plane);
    void (*display)(void *sys, void *id);
    void (*prepare)(void *sys, vlc_fourcc_t fourcc, int width, int height, double sar, double fps);
    void *opaque;
};

static picture_pool_t *Pool  (vout_display_t *, unsigned);
static void           Display(vout_display_t *, picture_t *);
static int            Control(vout_display_t *, int, va_list);
static void           Manage (vout_display_t *);

static int            Lock(picture_t *);
static void           Unlock(picture_t *);

/*****************************************************************************
 * Open: allocates video thread
 *****************************************************************************
 * This function allocates and initializes a vout method.
 *****************************************************************************/
static int Open(vlc_object_t *object)
{
    vout_display_t *vd = (vout_display_t *)object;

    /* */
    char *chroma_format = var_CreateGetString(vd, MODULE_STRING"-chroma");
    const vlc_fourcc_t chroma = vlc_fourcc_GetCodecFromString(VIDEO_ES, chroma_format);
    free(chroma_format);
    if (!chroma) {
        msg_Err(vd, "cmplayer-vout-chroma should be 4 characters long");
        return VLC_EGENERIC;
    }

    /* */
    video_format_t fmt = vd->fmt;

    fmt.i_chroma = chroma;

    /* Define the bitmasks */
    switch (chroma)
    {
    case VLC_CODEC_RGB15:
        fmt.i_rmask = 0x001f;
        fmt.i_gmask = 0x03e0;
        fmt.i_bmask = 0x7c00;
        break;
    case VLC_CODEC_RGB16:
        fmt.i_rmask = 0x001f;
        fmt.i_gmask = 0x07e0;
        fmt.i_bmask = 0xf800;
        break;
    case VLC_CODEC_RGB24:
        fmt.i_rmask = 0xff0000;
        fmt.i_gmask = 0x00ff00;
        fmt.i_bmask = 0x0000ff;
        break;
    case VLC_CODEC_RGB32:
        fmt.i_rmask = 0xff0000;
        fmt.i_gmask = 0x00ff00;
        fmt.i_bmask = 0x0000ff;
        break;
    default:
        fmt.i_rmask = 0;
        fmt.i_gmask = 0;
        fmt.i_bmask = 0;
        break;
    }

    vout_display_sys_t *sys;
    vd->sys = sys = calloc(1, sizeof(*sys));
    if (unlikely(!sys))
        return VLC_ENOMEM;

	char *psz_tmp = 0;
    psz_tmp = var_CreateGetString(vd, MODULE_STRING "-cb-lock" );
	sys->lock = (void *(*)(void *, void *, int))(intptr_t)atoll(psz_tmp);
    free(psz_tmp);

    if (sys->lock == NULL) {
        msg_Err(vd, "Invalid lock callback");
        free(sys);
        return VLC_EGENERIC;
    }

    psz_tmp = var_CreateGetString(vd, MODULE_STRING "-cb-unlock" );
    sys->unlock = (void (*)(void *, void *, void *const *))(intptr_t)atoll(psz_tmp);
    free(psz_tmp);
    
	psz_tmp = var_CreateGetString(vd, MODULE_STRING "-cb-display" );
    sys->display = (void (*)(void *, void *))(intptr_t)atoll(psz_tmp);
    free(psz_tmp);
    
    psz_tmp = var_CreateGetString(vd, MODULE_STRING "-cb-prepare" );
    sys->prepare = (void (*)(void *, vlc_fourcc_t, int, int, double, double))(intptr_t)atoll(psz_tmp);
    free(psz_tmp);
    
    psz_tmp = var_CreateGetString(vd, MODULE_STRING "-data" );
    sys->opaque = (void *)(intptr_t)atoll(psz_tmp);
    free(psz_tmp);

    psz_tmp = var_CreateGetString(vd, MODULE_STRING "-event-handler" );
    sys->eventHandler = (EventHandler*)(intptr_t)atoll(psz_tmp);
    free(psz_tmp);

	if (sys->eventHandler) {
		sys->eventHandler->self = sys->eventHandler;
		sys->eventHandler->mousePressed = handleMousePressed;
		sys->eventHandler->mouseReleased = handleMouseReleased;
		sys->eventHandler->mouseMoved = handleMouseMoved;
		sys->eventHandler->vd = vd;
	}

    picture_t *picture = picture_NewFromFormat(&fmt);
    if (!picture) {
        free(sys);
        return VLC_EGENERIC;
    }

	picture->p_sys = malloc(sizeof(picture_sys_t));
	picture->p_sys->id = 0;
	picture->p_sys->sys = sys;
	
    picture_pool_configuration_t pool;
    memset(&pool, 0, sizeof(pool));
    pool.picture_count = 1;
    pool.picture       = &picture;
    pool.lock          = Lock;
    pool.unlock        = Unlock;
    sys->pool = picture_pool_NewExtended(&pool);
    if (!sys->pool) {
        picture_Release(picture);
        free(sys);
        return VLC_EGENERIC;
    }

    vout_display_info_t info = vd->info;
    info.has_hide_mouse = true;

    vd->fmt     = fmt;
    vd->info    = info;
    vd->pool    = Pool;
    vd->prepare = NULL;
    vd->display = Display;
    vd->control = Control;
    vd->manage  = Manage;

    vout_display_SendEventFullscreen(vd, false);
    vout_display_SendEventDisplaySize(vd, fmt.i_width, fmt.i_height, false);
    
    char fourcc[5] = {0};
    const double fps = (double)fmt.i_frame_rate/(double)fmt.i_frame_rate_base;
    const double sar = (double)vd->source.i_sar_num/(double)vd->source.i_sar_den;
    sys->prepare(sys->opaque, fmt.i_chroma, fmt.i_width, fmt.i_height, sar, fps);
    vlc_fourcc_to_char(vd->source.i_chroma, fourcc);
    printf("source: %s\n", fourcc); fflush(stdout);
    return VLC_SUCCESS;
}

static void Close(vlc_object_t *object)
{
    vout_display_t *vd = (vout_display_t *)object;
    vout_display_sys_t *sys = vd->sys;
	if (sys->eventHandler)
		sys->eventHandler->self = NULL;
    picture_pool_Delete(sys->pool);
    free(sys);
}

/* */
static picture_pool_t *Pool(vout_display_t *vd, unsigned count)
{
//	printf("pool\n"); fflush(stdout);
    VLC_UNUSED(count);
    return vd->sys->pool;
}

static void Display(vout_display_t *vd, picture_t *picture)
{
    vout_display_sys_t *sys = vd->sys;

//    assert(!picture_IsReferenced(picture));
    if (sys->display != NULL)
        sys->display(sys->opaque, picture->p_sys->id);
    picture_Release(picture);
}

static int Control(vout_display_t *vd, int query, va_list args)
{
//	printf("control\n"); fflush(stdout);
    switch (query) {
    case VOUT_DISPLAY_CHANGE_FULLSCREEN:
    case VOUT_DISPLAY_CHANGE_DISPLAY_SIZE: {
        const vout_display_cfg_t *cfg = va_arg(args, const vout_display_cfg_t *);
        if (cfg->display.width  != vd->fmt.i_width ||
            cfg->display.height != vd->fmt.i_height)
            return VLC_EGENERIC;
        if (cfg->is_fullscreen)
            return VLC_EGENERIC;
        return VLC_SUCCESS;
    }
    default:
        return VLC_EGENERIC;
    }
}
static void Manage(vout_display_t *vd)
{
//	printf("manage\n"); fflush(stdout);
    VLC_UNUSED(vd);
}



/* */
static int Lock(picture_t *picture)
{
//	printf("lock\n"); fflush(stdout);
    picture_sys_t *picsys = picture->p_sys;
    vout_display_sys_t *sys = picsys->sys;
    Plane planes[PICTURE_PLANE_MAX];
    
	int len = 0;
	for (int i=0; i<picture->i_planes; ++i) {
		const plane_t *const p = &picture->p[i];
		planes[i].dataPitch = p->i_pitch;
		planes[i].dataLines = p->i_lines;
		planes[i].framePitch = p->i_visible_pitch;
		planes[i].frameLines = p->i_visible_lines;
		len += p->i_pitch*p->i_lines;
	}

	picsys->id = sys->lock(sys->opaque, (void*)planes, len);

    for (int i = 0; i < picture->i_planes; i++)
        picture->p[i].p_pixels = planes[i].data;

    return VLC_SUCCESS;
}

static void Unlock(picture_t *picture)
{
//	printf("unlock\n"); fflush(stdout);
    picture_sys_t *picsys = picture->p_sys;
    vout_display_sys_t *sys = picsys->sys;

    void *planes[PICTURE_PLANE_MAX];

    for (int i = 0; i < picture->i_planes; i++)
        planes[i] = picture->p[i].p_pixels;

    if (sys->unlock != NULL)
        sys->unlock(sys->opaque, picsys->id, planes);
}
