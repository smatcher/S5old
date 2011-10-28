uniform sampler2D gbuffer_diffuse;
varying vec2 screen_pos;
uniform vec3 scene_ambient;

void main()
{
	vec4 final = texture2D(gbuffer_diffuse, screen_pos);
	gl_FragColor = final * vec4(scene_ambient,1.0);
}
