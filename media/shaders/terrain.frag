uniform sampler2D alpha_map;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;

varying vec3 normal, lightDir, eyeVec;

void main (void)
{
	/* Texture splatting */
	vec4 color;
	vec4 c1 = texture2D(tex1, gl_TexCoord[0].st);
	vec4 c2 = texture2D(tex2, gl_TexCoord[0].st);
	vec4 c3 = texture2D(tex3, gl_TexCoord[0].st);
	vec4 splatting = texture2D(alpha_map, gl_TexCoord[1].st);
	color.r = splatting.r*c1.r + splatting.g*c2.r + splatting.b*c3.r;
	color.g = splatting.r*c1.g + splatting.g*c2.g + splatting.b*c3.g;
	color.b = splatting.r*c1.b + splatting.g*c2.b + splatting.b*c3.b;
	color.a = 1.0f;
	
//	color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

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

	gl_FragColor = final_color;			
}

