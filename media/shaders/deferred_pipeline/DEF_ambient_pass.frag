uniform sampler2D gbuffer_diffuse;
varying vec2 screen_pos;
uniform vec3 scene_ambient;

void main()
{
	vec4 dif_sample = texture2D(gbuffer_diffuse, screen_pos);
	vec4 diffuse = vec4(dif_sample.rgb,1.0);
	float sky = dif_sample.a;
	gl_FragColor = diffuse * sky * vec4(scene_ambient,1.0) + diffuse * (1.0 - sky);
}
