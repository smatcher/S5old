uniform sampler2D input;
uniform vec2 screen_size;
varying vec2 screen_pos;

void main()
{
	gl_FragColor = (6.0 * texture2D(input, vec2(screen_pos.s, screen_pos.t)) +
						 4.0 * texture2D(input, vec2(screen_pos.s - 1.3/screen_size.s, screen_pos.t)) +
						 4.0 * texture2D(input, vec2(screen_pos.s + 1.3/screen_size.s, screen_pos.t))) / 14.0;
}
