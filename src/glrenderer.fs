uniform sampler2D bgtex[3];
uniform float brightness, contrast, coshue, sinhue, saturation;

void main(void) {
	vec2 coord = gl_TexCoord[0].xy;
	const float y0 = 0.0625;
	const float uv0 = 0.5;
	float sc = contrast*saturation;
	float y = texture2D(bgtex[0], coord).r;
	y -= y0;
	y *= contrast;
	y += brightness;


	float _u = texture2D(bgtex[1], coord).r;
	float _v = texture2D(bgtex[2], coord).r;
	_u -= uv0;
	_v -= uv0;
	float u = (_u*coshue + _v*sinhue);
	float v = (-_u*sinhue + _v*coshue );
	u *= sc;
	v *= sc;
	y *= 1.164;
	gl_FragColor.r = y			+ 1.596*v;
	gl_FragColor.g = y	- 0.391*u	- 0.81300002*v;
	gl_FragColor.b = y	+ 2.0179999*u;
}

