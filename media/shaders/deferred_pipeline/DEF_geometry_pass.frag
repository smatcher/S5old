#ifdef COLOR_MAP
	uniform sampler2D colormap;
#else
	#ifdef SPLATTING
	uniform sampler2D splatting;
	uniform sampler2D splat_r;
	uniform sampler2D splat_g;
	uniform sampler2D splat_b;
	#endif
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
		#ifdef SKY
			gl_FragData[1] = texture2D(colormap, gl_TexCoord[0].st);
		#else
			gl_FragData[1] = texture2D(colormap, gl_TexCoord[0].st) * gl_FrontMaterial.diffuse;
		#endif
	#else
		#ifdef SPLATTING
			vec4 color;
			vec4 c1 = texture2D(splat_r, gl_TexCoord[0].st);
			vec4 c2 = texture2D(splat_g, gl_TexCoord[0].st);
			vec4 c3 = texture2D(splat_b, gl_TexCoord[0].st);
			vec4 splat = texture2D(splatting, gl_TexCoord[1].st);
			float sum = splat.r + splat.g + splat.b;
			color.r = (splat.r*c1.r + splat.g*c2.r + splat.b*c3.r)/sum;
			color.g = (splat.r*c1.g + splat.g*c2.g + splat.b*c3.g)/sum;
			color.b = (splat.r*c1.b + splat.g*c2.b + splat.b*c3.b)/sum;
			color.a = 1.0;
			gl_FragData[1] = color * gl_FrontMaterial.diffuse;
		#else
			gl_FragData[1] = gl_FrontMaterial.diffuse;
		#endif
	#endif
	#ifdef SKY
		gl_FragData[1].a = 0.0;
	#else
		gl_FragData[1].a = 1.0;
	#endif
	gl_FragData[2] = gl_FrontMaterial.specular;
	gl_FragData[2].a = gl_FrontMaterial.shininess;
}
