varying vec2 screen_pos;

void main()
{
	screen_pos = vec2(0.5,0.5) + gl_Vertex.st/2.0;
	gl_Position = ftransform();		
}

