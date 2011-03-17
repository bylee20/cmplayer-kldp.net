uniform sampler2D bgtex[3];
uniform float brightness, contrast, coshue, sinhue, saturation;

void main(void) {
	vec2 coord = gl_TexCoord[0].xy;
	const float y0 = 0.0625;
	const float uv0 = 0.5;
	float sc = contrast*saturation;
	float y = texture2D(bgtex[0], coord).r;
	y -= y0;


	float _u = texture2D(bgtex[1], coord).r;
	float _v = texture2D(bgtex[2], coord).r;
	_u -= uv0;
	_v -= uv0;
	float u = (_u*coshue + _v*sinhue);
	float v = (-_u*sinhue + _v*coshue );

	y *= contrast;
	u *= sc;
	v *= sc;

	y += brightness;
	y *= 1.164;

	gl_FragColor.r = y			+ 1.596*v;
	gl_FragColor.g = y	- 0.391*u	- 0.81300002*v;
	gl_FragColor.b = y	+ 2.0179999*u;
	gl_FragColor.a = 1.0;
}


glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 0.0f, d->saturation*d->contrast
	, d->contrast, d->coshue, d->sinhue);
glProgramLocalParameter4fARB(GL_FRAGMENT_PROGRAM_ARB, 1.0f, d->brightness
	, 1. /*alpha */, 0. /*dummy*/, 0. /*dummy*/);

!!ARBfp1.0
PARAM param0 = program.local[0]; # {contrast, saturation*contrast, saturation*contrast, brightness};
PARAM param1 = program.local[1]; # {coshue, sinhue, alpha, 0};
PARAM coef_r = {1.164,  0.0,       1.1596,     0.0625};
PARAM coef_g = {1.164, -0.391,    -0.81300002, 0.5};
PARAM coef_b = {1.164,  2.0179999, 0.0,        0.5};
TEMP yuv;
TEX yuv.x, fragment.texcoord[0], texture[1], 2D; #u
SUB yuv.z, yuv.x, coef_g.a; # u - 0.5
TEX yuv.x, fragment.texcoord[0], texture[2], 2D; #v
SUB yuv.w, yuv.x, coef_b.a; # v - 0.5; xy free
MUL yuv.x, yuv.z, param1.x; # u*coshue
MUL yuv.y, yuv.w, param1.y; # v*sinhue
ADD yuv.y, yuv.x, yuv.y; # u' = u + v; x free
MUL yuv.x, yuv.z, param1.y; # u*sinhue
MUL yuv.w, yuv.w, param1.x; # v*coshue
SUB yuv.z, yuv.w, yuv.x; # v' = v - u; xw free
TEX yuv.x, fragment.texcoord[0], texture[0], 2D; #y
SUB yuv.x, yuv, coef_r.a;
MUL yuv, yuv, param0;
ADD yuv.x, yuv, param0.w;
DP3 result.color.r, yuv, coef_r;
DP3 result.color.g, yuv, coef_g;
DP3 result.color.b, yuv, coef_b;
MOV result.color.a, 1.0;





PARAM c[5] = {
	program.local[0],
	program.local[1],
	{ 1.164, 0, 1.596, 0.5 },
	{ 0.0625, 1.164, -0.391, -0.81300002 },
	{ 1.164, 2.0179999, 0 }
};
TEMP R0;
TEX R0.x, fragment.texcoord[0], texture[1], 2D;
ADD R0.y, R0.x, -c[2].w;
TEX R0.x, fragment.texcoord[0], texture[2], 2D;
ADD R0.x, R0, -c[2].w;
MUL R0.z, R0.y, c[0].w;
MAD R0.z, R0.x, c[0], R0;
MUL R0.w, R0.x, c[0];
MUL R0.z, R0, c[0].y;
TEX R0.x, fragment.texcoord[0], texture[0], 2D;
MAD R0.y, R0, c[0].z, R0.w;
ADD R0.x, R0, -c[3];
MUL R0.y, R0, c[0];
MUL R0.z, R0, c[1].x;
MAD R0.x, R0, c[0].y, c[0];
MUL R0.y, R0, c[1].x;

DP3 result.color.x, R0, c[2];
DP3 result.color.y, R0, c[3].yzww;
DP3 result.color.z, R0, c[4];
MOV result.color.w, c[1].y;
END;
