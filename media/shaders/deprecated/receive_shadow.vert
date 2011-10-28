varying vec4 shadowcoord0, shadowcoord1, shadowcoord2, shadowcoord3, shadowcoord4, shadowcoord5;
uniform mat4 inverse_modelview;

void main()
{	
	vec4 transformed = inverse_modelview * gl_ModelViewMatrix * gl_Vertex;

	shadowcoord0 = gl_TextureMatrix[1] * transformed;
	shadowcoord1 = gl_TextureMatrix[2] * transformed;
	shadowcoord2 = gl_TextureMatrix[3] * transformed;
	shadowcoord3 = gl_TextureMatrix[4] * transformed;
	shadowcoord4 = gl_TextureMatrix[5] * transformed;
	shadowcoord5 = gl_TextureMatrix[6] * transformed;
	gl_Position = ftransform();
}

