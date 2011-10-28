uniform sampler2D color_map;
varying vec3 normal, lightDir, eyeVec;

void main (void)
{
	vec4 color = texture2D(color_map, gl_TexCoord[0].st);

	vec4 final_color = color * (
	(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + 
	(gl_LightSource[0].ambient * gl_FrontMaterial.ambient));
							
	vec3 N = normalize(normal);
	vec3 L = normalize(lightDir);
	
	float lambertTerm = dot(N,L);
	
	if(lambertTerm > 0.0)
	{
		final_color += color * (
							gl_LightSource[0].diffuse * 
		               gl_FrontMaterial.diffuse * 
					   lambertTerm);	
		
		vec3 E = normalize(eyeVec);
		vec3 R = reflect(-L, N);
		float specular = pow( max(dot(R, E), 0.0), 
		                 gl_FrontMaterial.shininess );
		final_color += gl_LightSource[0].specular * 
		               gl_FrontMaterial.specular * 
					   specular;	
	}

	//if(N.z < 0.8 && N.z > -0.8)
	//{
	//	float rapport = 1-(abs(N.z)/0.8);
		//float rapport = 1-abs(N.z);
		float rapport = 0.8*abs(N.z);
		rapport = rapport * rapport;
		vec4 outline_color = vec4(0.0,1.0,0.0,1.0);
		final_color = (1-rapport) * final_color + rapport * outline_color;
	//}

	gl_FragColor = final_color;			
}

