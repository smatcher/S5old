varying vec3 normal;

#ifdef NORMAL_MAP
	varying vec3 eyetangent;
	varying vec3 eyebitangent;
	attribute vec3 tangent;
	attribute vec3 bitangent;
#endif

void main()
{	
	gl_TexCoord[0] = gl_MultiTexCoord0;

	normal = gl_NormalMatrix * gl_Normal;

	#ifdef NORMAL_MAP
		eyetangent = gl_NormalMatrix * tangent;
		eyebitangent = gl_NormalMatrix * bitangent;
	#endif

	gl_Position = ftransform();		
}

