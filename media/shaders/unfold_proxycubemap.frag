uniform sampler2D map0;
uniform sampler2D map1;
uniform sampler2D map2;
uniform sampler2D map3;
uniform sampler2D map4;
uniform sampler2D map5;

void main()
{
	vec4 color = vec4(0.0, 0.0, 0.0, 1.0); 
	float s = gl_TexCoord[0].s * 3.0;
	float t = gl_TexCoord[0].t * 3.0;
	if(s < 1.0)
	{
		if(t < 1.0)
		{
			color = texture2D(map5, vec2(s,t));
		}
		else if(t < 2.0)
		{
			color = texture2D(map0, vec2(s,t-1.0));
		}
	}
	else if(s < 2.0)
	{
		if(t < 1.0)
		{
			color = texture2D(map3, vec2(s-1.0,t));
		}
		else if(t < 2.0)
		{
			color = texture2D(map4, vec2(s-1.0,t-1.0));
		}
		else
		{
			color = texture2D(map2, vec2(s-1.0,t-2.0));
		}
	}
	else
	{
		if(t > 1.0 && t < 2.0)
		{
			color = texture2D(map1, vec2(s-2.0,t-1.0));
		}
	}
	gl_FragColor = color;
}

