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

uniform vec3 scene_ambient;
varying vec3 normal;

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
		vec4 diffuse = texture2D(colormap, gl_TexCoord[0].st) * gl_FrontMaterial.diffuse;
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

		vec3 E = normalize(eyeVec);
		#ifdef LIGHT_OMNI_0
		{
			vec3 L0 = normalize(lightDir0);
			vec3 HalfVec0 = normalize(lightDir0 + eyeVec);

			float dist0 = length(lightDir0);
			float attenuation0 = 1.0/ (
					gl_LightSource[0].constantAttenuation +
					gl_LightSource[0].linearAttenuation*dist0 +
					gl_LightSource[0].quadraticAttenuation*dist0*dist0
			);

			vec3 idiff0 = clamp(dot(L0, N) * gl_LightSource[0].diffuse.rgb * diffuse.rgb, 0.0, 1.0);
			vec3 ispec0 = pow(clamp(dot(HalfVec0,N),0.0,1.0),gl_FrontMaterial.shininess) * gl_LightSource[0].specular.rgb * gl_FrontMaterial.specular.rgb;
			final_color.rgb += (idiff0+ispec0) * attenuation0;
		}
		#endif
		#ifdef LIGHT_OMNI_1
		{
			vec3 L1 = normalize(lightDir1);
			vec3 HalfVec1 = normalize(lightDir1 + eyeVec);

			float dist1 = length(lightDir1);
			float attenuation1 = 1.0/ (
					gl_LightSource[1].constantAttenuation +
					gl_LightSource[1].linearAttenuation*dist1 +
					gl_LightSource[1].quadraticAttenuation*dist1*dist1
			);

			vec3 idiff1 = clamp(dot(L1, N) * gl_LightSource[1].diffuse.rgb * diffuse.rgb, 0.0, 1.0);
			vec3 ispec1 = pow(clamp(dot(HalfVec1,N),0.0,1.0),gl_FrontMaterial.shininess) * gl_LightSource[1].specular.rgb * gl_FrontMaterial.specular.rgb;
			final_color.rgb += (idiff1+ispec1) * attenuation1;
		}
		#endif
		#ifdef LIGHT_OMNI_2
		{
			vec3 L2 = normalize(lightDir2);
			vec3 HalfVec2 = normalize(lightDir2 + eyeVec);

			float dist2 = length(lightDir2);
			float attenuation2 = 1.0/ (
					gl_LightSource[2].constantAttenuation +
					gl_LightSource[2].linearAttenuation*dist2 +
					gl_LightSource[2].quadraticAttenuation*dist2*dist2
			);

			vec3 idiff2 = clamp(dot(L2, N) * gl_LightSource[2].diffuse.rgb * diffuse.rgb, 0.0, 1.0);
			vec3 ispec2 = pow(clamp(dot(HalfVec2,N),0.0,1.0),gl_FrontMaterial.shininess) * gl_LightSource[2].specular.rgb * gl_FrontMaterial.specular.rgb;
			final_color.rgb += (idiff2+ispec2) * attenuation2;

		}
		#endif
		#ifdef LIGHT_OMNI_3
		{
			vec3 L3 = normalize(lightDir3);
			vec3 HalfVec3 = normalize(lightDir3 + eyeVec);

			float dist3 = length(lightDir3);
			float attenuation3 = 1.0/ (
					gl_LightSource[3].constantAttenuation +
					gl_LightSource[3].linearAttenuation*dist3 +
					gl_LightSource[3].quadraticAttenuation*dist3*dist3
			);

			vec3 idiff3= clamp(dot(L3, N) * gl_LightSource[3].diffuse.rgb * diffuse.rgb, 0.0, 1.0);
			vec3 ispec3= pow(clamp(dot(HalfVec3,N),0.0,1.0),gl_FrontMaterial.shininess) * gl_LightSource[3].specular.rgb * gl_FrontMaterial.specular.rgb;
			final_color.rgb += (idiff3+ispec3) * attenuation3;
		}
		#endif
	#endif //SKY

	#ifdef BLOOM
	//	gl_FragData[0] = vec4((idiff+ispec) * attenuation * diffuse.a, 1.0);
	//	gl_FragData[1] = vec4((ispec) * attenuation * diffuse.a , 1.0);
	#else
		gl_FragColor = final_color;
	#endif
}
