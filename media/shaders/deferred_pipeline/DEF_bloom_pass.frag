uniform sampler2D colormap;
uniform sampler2D bloommap;
uniform float bloom_factor;
varying vec2 screen_pos;

void main()
{
	gl_FragColor = texture2D(colormap, vec2(screen_pos.s, screen_pos.t)) + bloom_factor * texture2D(bloommap, vec2(screen_pos.s, screen_pos.t));
}
