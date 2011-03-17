/*****************************************************************************
 * normvol.c: volume normalizer
 *****************************************************************************
 * Copyright (C) 2001, 2006 the VideoLAN team
 * $Id: 6a9dc2700bb1c0023a563a834b6c0380d7b3ffa6 $
 *
 * Authors: Clément Stenac <zorglub@videolan.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#define __PLUGIN__
#define MODULE_STRING "cmplayer-afilter"

// gcc -I/Applications/VLC.app/Contents/MacOS/include -I. -L/Applications/VLC.app/Contents/MacOS/lib -std=c99 -lvlc -lvlccore -dynamiclib  cmplayer-afilter.c -o libcmplayer-afilter_plugin.dylib

/*
 * TODO:
 *
 * We should detect fast power increases and react faster to these
 * This way, we can increase the buffer size to get a more stable filter */


/*****************************************************************************
 * Preamble
 *****************************************************************************/

#include <math.h>

#include <vlc_common.h>
#include <vlc_plugin.h>

#include <vlc_aout.h>
#include <vlc_filter.h>

/*****************************************************************************
 * Local prototypes
 *****************************************************************************/

static int  Open     ( vlc_object_t * );
static void Close    ( vlc_object_t * );
static block_t *DoWork( filter_t *, block_t * );

struct filter_sys_t
{
//    int i_nb;
//    float *p_last;
//    float f_max;
	void (*doWork)(void *data, int samples, float *buffer);
    void (*prepare)(void *data, int nb_channel);
    void *opaque;
};

/*****************************************************************************
 * Module descriptor
 *****************************************************************************/
#define BUFF_TEXT N_("Number of audio buffers" )
#define BUFF_LONGTEXT N_("This is the number of audio buffers on which the " \
                "power measurement is made. A higher number of buffers will " \
                "increase the response time of the filter to a spike " \
                "but will make it less sensitive to short variations." )

#define LEVEL_TEXT N_("Maximal volume level" )
#define LEVEL_LONGTEXT N_("If the average power over the last N buffers " \
               "is higher than this value, the volume will be normalized. " \
               "This value is a positive floating point number. A value " \
               "between 0.5 and 10 seems sensible." )

vlc_module_begin ()
    set_description(MODULE_STRING)
    set_shortname(MODULE_STRING)
    set_category( CAT_AUDIO )
    set_subcategory( SUBCAT_AUDIO_AFILTER )
    set_capability( "audio filter", 0 )
    add_shortcut( MODULE_STRING )
    
	add_string(MODULE_STRING "-cb-prepare", "0", NULL, "", "", true)
		change_volatile()
    add_string(MODULE_STRING "-cb-do-work", "0", NULL, "", "", true)
        change_volatile()
    add_string(MODULE_STRING "-data", "0", NULL, "", "", true)
        change_volatile()
    
//    add_integer( MODULE_STRING"-buff-size", 20 ,NULL ,BUFF_TEXT, BUFF_LONGTEXT,
  //               true )
    //add_float( "norm-max-level", 2.0, NULL, LEVEL_TEXT,
      //         LEVEL_LONGTEXT, true )
    set_callbacks( Open, Close )
vlc_module_end ()

/*****************************************************************************
 * Open: initialize and create stuff
 *****************************************************************************/
static int Open( vlc_object_t *p_this )
{
    filter_t *p_filter = (filter_t*)p_this;
    unsigned i_channels;
    filter_sys_t *p_sys;

    if( p_filter->fmt_in.audio.i_format != VLC_CODEC_FL32 ||
        p_filter->fmt_out.audio.i_format != VLC_CODEC_FL32 )
    {
        p_filter->fmt_in.audio.i_format = VLC_CODEC_FL32;
        p_filter->fmt_out.audio.i_format = VLC_CODEC_FL32;
        msg_Warn( p_filter, "bad input or output format" );
        return VLC_EGENERIC;
    }

    if ( !AOUT_FMTS_SIMILAR( &p_filter->fmt_in.audio, &p_filter->fmt_out.audio ) )
    {
        memcpy( &p_filter->fmt_out.audio, &p_filter->fmt_in.audio,
                sizeof(audio_sample_format_t) );
        msg_Warn( p_filter, "input and output formats are not similar" );
        return VLC_EGENERIC;
    }

    p_filter->pf_audio_filter = DoWork;

	p_sys = p_filter->p_sys = malloc( sizeof( *p_sys ) );
    if( !p_sys )
        return VLC_ENOMEM;
        
//    p_sys->i_nb = var_CreateGetInteger( p_filter->p_parent, "norm-buff-size" );
//    p_sys->f_max = var_CreateGetFloat( p_filter->p_parent, "norm-max-level" );
//    if( p_sys->f_max <= 0 ) p_sys->f_max = 0.01;

	char *psz_tmp = 0;
    psz_tmp = var_CreateGetString(p_filter->p_parent, MODULE_STRING "-cb-do-work" );
	p_sys->doWork = (void (*)(void *, int, float *))(intptr_t)atoll(psz_tmp);
    free(psz_tmp);
    
    psz_tmp = var_CreateGetString(p_filter->p_parent, MODULE_STRING "-cb-prepare" );
    p_sys->prepare = (void (*)(void *, int))(intptr_t)atoll(psz_tmp);
    free(psz_tmp);
    
    psz_tmp = var_CreateGetString(p_filter->p_parent, MODULE_STRING "-data" );
    p_sys->opaque = (void *)(intptr_t)atoll(psz_tmp);
    free(psz_tmp);

    i_channels = aout_FormatNbChannels( &p_filter->fmt_in.audio );
	p_sys->prepare(p_sys->opaque, i_channels);

    /* We need to store (nb_buffers+1)*nb_channels floats */
//    p_sys->p_last = calloc( i_channels * (p_filter->p_sys->i_nb + 2), sizeof(float) );
//    if( !p_sys->p_last )
//    {
//        free( p_sys );
//        return VLC_ENOMEM;
//    }

    return VLC_SUCCESS;
}

/*****************************************************************************
 * DoWork : normalizes and sends a buffer
 *****************************************************************************/
static block_t *DoWork( filter_t *p_filter, block_t *p_in_buf )
{
    int i_samples = p_in_buf->i_nb_samples;
    int i_channels = aout_FormatNbChannels( &p_filter->fmt_in.audio );
    float *p_in =  (float*)p_in_buf->p_buffer;

    struct filter_sys_t *p_sys = p_filter->p_sys;
	p_sys->doWork(p_sys->opaque, i_samples, p_in);

    return p_in_buf;
}

/**********************************************************************
 * Close
 **********************************************************************/
static void Close( vlc_object_t *p_this )
{
    filter_t *p_filter = (filter_t*)p_this;
    filter_sys_t *p_sys = p_filter->p_sys;
    free( p_sys );
}
