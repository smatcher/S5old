uniform sampler2D color_map;

void main (void)
{
	gl_FragColor = texture2D(color_map, gl_TexCoord[0].st);
}

