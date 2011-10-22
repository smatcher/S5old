// blur.frag

uniform sampler2D tex_source;
uniform sampler2D to_blur;
uniform float offset;
uniform float glow_factor;

void main()
{
	gl_FragColor = texture2D(tex_source, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t)) +
						(glow_factor * 0.2 * (
						texture2D(to_blur, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t)) +
						texture2D(to_blur, vec2(gl_TexCoord[0].s - offset, gl_TexCoord[0].t)) +
						texture2D(to_blur, vec2(gl_TexCoord[0].s + offset, gl_TexCoord[0].t)) +
						texture2D(to_blur, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t - offset)) +
						texture2D(to_blur, vec2(gl_TexCoord[0].s, gl_TexCoord[0].t + offset))));
}
