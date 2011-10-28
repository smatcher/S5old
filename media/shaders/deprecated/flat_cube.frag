uniform samplerCube color_map;
varying vec3 refdir;

void main (void)
{
	vec4 color = textureCube(color_map, refdir);
	gl_FragColor = color;
}

