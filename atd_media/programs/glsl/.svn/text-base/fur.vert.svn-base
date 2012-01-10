// fur.vert

uniform float layer;
uniform float fur_lenght;
uniform float fur_repetition;
//attribute vec3 normal;

void main()
{
	gl_TexCoord[0]=gl_MultiTexCoord0;
	gl_TexCoord[1]=gl_MultiTexCoord1*0.0001+gl_MultiTexCoord0*fur_repetition;
	//gl_TexCoord[1]=gl_MultiTexCoord1*0.0001+ftransform();
	gl_TexCoord[2]=gl_MultiTexCoord0*0.0001+gl_MultiTexCoord0;
	gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex+vec4(gl_Normal,0)*layer*fur_lenght);
}

