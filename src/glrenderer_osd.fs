uniform sampler2D tex;
uniform vec2 tc_offset[9];

void main(void) {
	vec3 blur_color = vec3(1.0, 1.0, 1.0);
	vec4 sample[9];
	for (int i=0; i<9; ++i) {
		sample[i].a = texture2D(tex, gl_TexCoord[0].xy + tc_offset[i]).a;
		sample[i].xyz = blur_color*sample[i].a;
	}
	vec4 img =
		sample[0] +	2.0*sample[1] +	sample[2] +
		2.0*sample[3] +	sample[4] +	2.0*sample[5] +
		sample[6] +	2.0*sample[7] +	sample[8];
	img /= 13.0;
	vec4 over = texture2D(tex, gl_TexCoord[0].xy);
	if (over.a > 0.0) {
		img.rgb = img.rgb*(1.0 - over.a) + over.rgb;
	}
	gl_FragColor = img;
}
