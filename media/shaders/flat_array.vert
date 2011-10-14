varying vec3 refdir;

void main()
{	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	vec4 camcen = gl_ModelViewMatrixInverse[3];
	vec4 dir = gl_Vertex - camcen;
	refdir = reflect(dir.xyz, gl_Normal);
	gl_Position = ftransform();		
}

