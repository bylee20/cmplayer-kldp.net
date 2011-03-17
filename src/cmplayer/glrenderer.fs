uniform sampler2D tex0, tex1, tex2;
uniform float brightness, contrast, coshue, sinhue, saturation;

void main(void) {
	vec2 coord = gl_TexCoord[0].xy;
	vec3 bg = vec3(0.0, 0.0, 0.0);
	if (0.0 <= coord.x && coord.x <= 1.0 && 0.0 <= coord.y && coord.y <= 1.0) {
		const float y0 = 0.0625;
		const float uv0 = 0.5;
		float sc = contrast*saturation;
		float y = texture2D(tex0, coord).r;
		float _u = texture2D(tex1, coord).r;
		float _v = texture2D(tex2, coord).r;
		y -= y0;
		y *= contrast;
		y += brightness;
		_u -= uv0;
		_v -= uv0;
		float u = (_u*coshue + _v*sinhue);
		float v = (_v*coshue - _u*sinhue);
		u *= sc;
		v *= sc;
		y *= 1.164;
		bg.r = y + 1.596*v;
		bg.g = y - 0.81300002*v - 0.391*u;
		bg.b = y + 2.0179999*u;
	}
	gl_FragColor.xyz = bg;
	gl_FragColor.w = 1.0;

}

