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

#ifdef SSS_FINAL
	uniform sampler2D sss1;
	uniform sampler2D sss2;
	uniform sampler2D sss3;
#endif
#ifdef SSS_MAP
	uniform sampler2D sssmap;
#endif


uniform vec3 scene_ambient;
varying vec3 normal;

varying vec3 eyeVec;

#for 0 7
	#if defined LIGHT_OMNI_@ || defined LIGHT_SPOT_@ || defined LIGHT_SUN_@
		varying vec3 lightDir@;
	#endif
#endfor

void main()
{
	#ifdef NORMAL_MAP
		vec3 eyespacenormal = vec3(1.0,1.0,1.0);
		vec3 tanspacenormal = texture2D(normalmap, gl_TexCoord[0].st).rgb * 2.0 - 1.0;
		eyespacenormal =  tanspacenormal.x * eyetangent;
		eyespacenormal += tanspacenormal.y * eyebitangent;
		eyespacenormal += tanspacenormal.z * normal;

		vec3 N = normalize(eyespacenormal);
	#else
		vec3 N = normalize(normal);
	#endif

	#ifdef COLOR_MAP
		#ifdef SKY
			vec4 diffuse = texture2D(colormap, gl_TexCoord[0].st);
		#else
			#ifdef GRADIENT_MAP
				float gradient = texture2D(colormap, gl_TexCoord[0].st).r;
				vec4 diffuse = texture2D(gradientmap, vec2(gradient,0.0)) * gl_FrontMaterial.diffuse;
			#else
				vec4 diffuse = texture2D(colormap, gl_TexCoord[0].st) * gl_FrontMaterial.diffuse;
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
			vec4 diffuse = color * gl_FrontMaterial.diffuse;
		#else
			vec4 diffuse = gl_FrontMaterial.diffuse;
		#endif
	#endif

	#ifdef SKY
		vec4 final_color = diffuse;
	#else
		vec4 final_color = vec4(scene_ambient,1.0) * diffuse;

		vec3 matSpec = gl_FrontMaterial.specular.rgb;
		float matShininess = gl_FrontMaterial.shininess;
		#ifdef SPECULAR_MAP && !SSS_PREPASS
			vec4 specMap = texture2D(specularmap, gl_TexCoord[0].st);
			matSpec *= specMap.rgb;
			matShininess *= specMap.a;
		#endif

		vec3 E = normalize(eyeVec);
		#for 0 7
			#if defined LIGHT_OMNI_@ || defined LIGHT_SPOT_@ || defined LIGHT_SUN_@
			{
				vec3 L@ = normalize(lightDir@);

				float dist@ = length(lightDir@);
				float attenuation@ = 1.0/ (
						gl_LightSource[@].constantAttenuation +
						gl_LightSource[@].linearAttenuation*dist@ +
						gl_LightSource[@].quadraticAttenuation*dist@*dist@
				);

				#ifdef LIGHT_SPOT_@
					float spotAngle@ = dot(L@,normalize(gl_LightSource[@].spotDirection));
					if(spotAngle@ < gl_LightSource[@].spotCutoff)
						attenuation@ = 0.0;
				#endif

				vec3 idiff@ = clamp(dot(L@, N) * gl_LightSource[@].diffuse.rgb * diffuse.rgb, 0.0, 1.0);

				#ifdef SSS_PREPASS
					final_color.rgb += (idiff@) * attenuation@;
				#else
					vec3 HalfVec@ = normalize(L@ + E);
					vec3 ispec@ = pow(clamp(dot(HalfVec@,N),0.0,1.0),matShininess) * gl_LightSource[@].specular.rgb * matSpec;
					final_color.rgb += (idiff@+ispec@) * attenuation@;
				#endif
			}
			#endif
		#endfor
	#endif //SKY

	#ifndef SSS_FINAL
		gl_FragColor = final_color;
	#else
		vec4 ratio = vec4(1.0, 1.0, 1.0, 0.2);
		#ifdef SSS_MAP
			ratio = texture2D(sssmap, gl_TexCoord[0].st);
		#endif
			vec3 SSScontribution = mix(texture2D(sss3, gl_TexCoord[0].st).rgb, vec3(0.0, 0.0, 0.0), ratio.a);
			SSScontribution = mix(texture2D(sss2, gl_TexCoord[0].st).rgb, SSScontribution * ratio.rgb, ratio.a);
			SSScontribution = mix(texture2D(sss1, gl_TexCoord[0].st).rgb, SSScontribution * ratio.rgb,ratio.a);
			gl_FragColor = vec4(mix(final_color.rgb,SSScontribution * ratio.rgb, ratio.a),final_color.a);//(ratio.r * final_color + ratio.g * layer1 + ratio.b * layer2 + ratio.a * layer3) / sum;
/*
//				vec4 layer1 = texture2D(sss1, gl_TexCoord[0].st);
	//			vec4 layer2 = texture2D(sss2, gl_TexCoord[0].st);
		//		vec4 layer3 = texture2D(sss3, gl_TexCoord[0].st);
			//	float sum = ratio.r + ratio.g + ratio.b + ratio.a;
		//gl_FragColor = texture2D(sss1, gl_TexCoord[0].st);
*/
	#endif
}
