varying vec3 normal;

#ifdef NORMAL_MAP
	varying vec3 eyetangent;
	varying vec3 eyebitangent;
	attribute vec3 tangent;
	attribute vec3 bitangent;
#endif

varying vec3 eyeVec;

#ifdef LIGHT_OMNI_0
	varying vec3 lightDir0;
#endif
#ifdef LIGHT_OMNI_1
	varying vec3 lightDir1;
#endif
#ifdef LIGHT_OMNI_2
	varying vec3 lightDir2;
#endif
#ifdef LIGHT_OMNI_3
	varying vec3 lightDir3;
#endif

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;
	#ifdef SPLATTING
		gl_TexCoord[1] = gl_MultiTexCoord1;
	#endif

	normal = normalize(gl_NormalMatrix * gl_Normal);

	#ifdef NORMAL_MAP
		eyetangent = normalize(gl_NormalMatrix * tangent);
		eyebitangent = normalize(gl_NormalMatrix * bitangent);
	#endif

	vec3 vVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	eyeVec = -vVertex;

	#ifdef LIGHT_OMNI_0
		lightDir0 = vec3(gl_LightSource[0].position.xyz - vVertex);
	#endif
	#ifdef LIGHT_OMNI_1
		lightDir1 = vec3(gl_LightSource[1].position.xyz - vVertex);
	#endif
	#ifdef LIGHT_OMNI_2
		lightDir2 = vec3(gl_LightSource[2].position.xyz - vVertex);
	#endif
	#ifdef LIGHT_OMNI_3
		lightDir3 = vec3(gl_LightSource[3].position.xyz - vVertex);
	#endif

	gl_Position = ftransform();
}

