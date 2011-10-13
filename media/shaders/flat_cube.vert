varying vec3 normal, lightDir, eyeVec, vCoords;

void main()
{	
	gl_TexCoord[0] = gl_MultiTexCoord0;

	normal = gl_NormalMatrix * gl_Normal;

	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);

	lightDir = vec3(gl_LightSource[0].position.xyz - vVertex);
	eyeVec = -vVertex;

	vec4 vCoords_ = vec4(normal, 0.0); //vec4(reflect(eyeVec, normal), 0.0);
	vCoords = (gl_ModelViewMatrixInverse * vCoords_).xyz;

	gl_Position = ftransform();		
}

