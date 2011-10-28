uniform sampler2D map0, map1, map2, map3, map4, map5;
varying vec3 refdir;
void main (void)
{
	vec4 color = 0.2*texture2D(map0, gl_TexCoord[0].st);
	color += 0.2*texture2D(map1, gl_TexCoord[0].st);
	color += 0.2*texture2D(map2, gl_TexCoord[0].st);
	color += 0.2*texture2D(map3, gl_TexCoord[0].st);
	color += 0.2*texture2D(map4, gl_TexCoord[0].st);
	color += 0.2*texture2D(map5, gl_TexCoord[0].st);
	gl_FragColor = color;
}

