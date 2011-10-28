#ifdef COLOR_MAP
	uniform sampler2D colormap;
#endif

#ifdef NORMAL_MAP
	varying vec3 eyetangent;
	varying vec3 eyebitangent;

	uniform sampler2D normalmap;
#endif

varying vec3 normal;

void main()
{
	#ifdef NORMAL_MAP
		vec3 eyespacenormal = vec3(1.0,1.0,1.0);
    	vec3 tanspacenormal = texture2D(normalmap, gl_TexCoord[0].st).rgb * 2.0 - 1.0;
		eyespacenormal =  tanspacenormal.x * eyetangent;
		eyespacenormal += tanspacenormal.y * eyebitangent;
		eyespacenormal += tanspacenormal.z * normal;

		gl_FragData[0] = vec4(normalize(eyespacenormal),1.0);
	#else
		gl_FragData[0] = vec4(normalize(normal),1.0);
	#endif
	#ifdef COLOR_MAP
		gl_FragData[1] = texture2D(colormap, gl_TexCoord[0].st) * gl_FrontMaterial.diffuse;
		gl_FragData[1].a = gl_FrontMaterial.shininess;
	#else
		gl_FragData[1] = gl_FrontMaterial.diffuse;
		gl_FragData[1].a = gl_FrontMaterial.shininess;
	#endif
	gl_FragData[2] = gl_FrontMaterial.specular;
}
