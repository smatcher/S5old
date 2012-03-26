#ifdef COLOR_MAP
	uniform sampler2D colormap;
	#ifdef GRADIENT_MAP
		uniform sampler2D gradientmap;
	#endif
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

#ifdef SPECULAR_MAP
	uniform sampler2D specularmap;
#endif

#ifdef LIGHT_SCATTERING
	uniform vec3 sky_color;
#endif

varying vec3 normal;

void main()
{
	#ifdef LIGHT_SCATTERING
		#ifdef LIGHT_SCATTERING_SUN
			gl_FragData[0] = vec4(0.0,0.0,0.0,0.0);
			gl_FragData[1] = vec4(0.0,0.0,0.0,0.0);
			gl_FragData[2] = vec4(0.0,0.0,0.0,0.0);
			gl_FragData[3] = vec4(1.0,1.0,1.0,1.0);
			return;
		#endif
	#endif

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
			#ifdef GRADIENT_MAP
				float gradient = texture2D(colormap, gl_TexCoord[0].st).r;
				gl_FragData[1] = texture2D(gradientmap, vec2(gradient,0.0)) * gl_FrontMaterial.diffuse;
			#else
				gl_FragData[1] = texture2D(colormap, gl_TexCoord[0].st) * gl_FrontMaterial.diffuse;
			#endif
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
		#ifdef LIGHT_SCATTERING
			//gl_FragData[3] = gl_FragData[1];
			gl_FragData[3] = vec4(sky_color,1.0);
		#endif
	#else
		gl_FragData[1].a = 1.0;
		#ifdef LIGHT_SCATTERING
			gl_FragData[3] = vec4(0.0,0.0,0.0,1.0);
		#endif
	#endif
	gl_FragData[2] = gl_FrontMaterial.specular;
	gl_FragData[2].a = gl_FrontMaterial.shininess/128.0;
	#ifdef SPECULAR_MAP
		vec4 specMap = texture2D(specularmap, gl_TexCoord[0].st);
		gl_FragData[2] *= gl_FrontMaterial.specular * specMap;
	#endif
}
