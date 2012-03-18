uniform float exposure;
uniform float decay;

uniform vec2 sun_pos;
varying vec2 screen_pos;

uniform sampler2D firstPass;

void main()
{
	vec2 deltaTextCoord = vec2(screen_pos - sun_pos);
	vec2 textCoo = screen_pos;
	deltaTextCoord *= 1.0 / 40.0;
	float illuminationDecay = 1.0;

	gl_FragColor = vec4(0.0,0.0,0.0,1.0);

	for(int i=0; i < 40 ; i++)
	{
		textCoo -= deltaTextCoord;
		vec4 sample = texture2D(firstPass, textCoo );
		sample *= illuminationDecay;
		gl_FragColor += sample;
		illuminationDecay *= decay;
	}

	gl_FragColor *= exposure;
}
