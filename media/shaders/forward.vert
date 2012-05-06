varying vec3 normal;

#ifdef NORMAL_MAP
	varying vec3 eyetangent;
	varying vec3 eyebitangent;
	attribute vec3 tangent;
	attribute vec3 bitangent;
#endif

varying vec3 eyeVec;
varying vec4 worldPos;
uniform mat4 inverse_view;

#for 0 7
	#if defined LIGHT_OMNI_@ || defined LIGHT_SPOT_@ || defined LIGHT_SUN_@
		varying vec3 lightDir@;
	#endif
#endfor

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

	vec4 vVertex = gl_ModelViewMatrix * gl_Vertex;
	eyeVec = -vVertex.xyz;
	worldPos = inverse_view * vVertex;

	#for 0 7
		#if defined LIGHT_OMNI_@ || defined LIGHT_SPOT_@ || defined LIGHT_SUN_@
			#ifdef LIGHT_SUN_@
				lightDir@ = normalize(gl_LightSource[@].spotDirection);
			#else
				lightDir@ = vec3(gl_LightSource[@].position.xyz - vVertex.xyz);
			#endif
		#endif
	#endfor

	#ifndef SSS_PREPASS
		gl_Position = ftransform();
	#else
		vec2 uv = gl_MultiTexCoord0.st;
		uv = vec2(-1.0,-1.0) + 2.0*uv;
		gl_Position = vec4(uv,0.0,1.0);
	#endif
}

