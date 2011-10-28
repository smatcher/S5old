// fur.frag

uniform float color;
uniform float layer;
uniform sampler2D color_map;
uniform sampler2D alpha_map;
uniform sampler2D fur_map;

varying vec4 diffuse_color;

void main()
{
	float alpha;
	if(length(texture2D(fur_map, gl_TexCoord[2].st).rgb)>layer)
		alpha = texture2D(alpha_map, gl_TexCoord[1].st).a;
	else
		alpha = 0.0;
	gl_FragColor =	texture2D(color_map, gl_TexCoord[0].st)*color*diffuse_color;
	gl_FragColor.a = alpha;

	//gl_FragColor=gl_FragColor*0.00001 + 0.5*texture2D(fur_map, gl_TexCoord[0].st) + 0.5*texture2D(color_map, gl_TexCoord[0].st);
}

