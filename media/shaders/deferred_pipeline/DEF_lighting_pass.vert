varying vec2 screen_pos;
varying vec3 eyelightpos;
uniform mat4 modelview;

void main()
{
	screen_pos = vec2(0.5,0.5) + gl_Vertex.st/2.0;
	eyelightpos = (modelview * gl_LightSource[0].position).xyz;

	gl_Position = ftransform();		
}

