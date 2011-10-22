uniform sampler2D shadowmap;
uniform sampler2D color_map;
uniform vec2 screen_size;
varying vec3 normal, lightDir, eyeVec;

void main (void)
{
	vec4 color = texture2D(color_map, gl_TexCoord[0].st);
	vec4 shadow = texture2D(shadowmap, gl_FragCoord.st / screen_size);

	vec4 final_color = color * (
	(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + 
	(gl_LightSource[0].ambient * gl_FrontMaterial.ambient));
							
	vec3 N = normalize(normal);
	vec3 L = normalize(lightDir);
	
	float lambertTerm = dot(N,L) * shadow.r;
	
	float specular_alpha = 0.0;
	vec4 specular_color = vec4(1.0, 0.0, 0.0, 1.0);
	if(lambertTerm > 0.0)
	{
		final_color += color * (
							gl_LightSource[0].diffuse * 
		               gl_FrontMaterial.diffuse * 
					   lambertTerm);	
		
		vec3 E = normalize(eyeVec);
		vec3 R = reflect(-L, N);
		specular_alpha = pow( max(dot(R, E), 0.0), 
		                 gl_FrontMaterial.shininess );
		specular_color = gl_LightSource[0].specular * 
		               gl_FrontMaterial.specular;
	}

	final_color += specular_alpha * specular_color;
	specular_color = final_color * specular_alpha;
	specular_color.a = 1.0;

	gl_FragData[0] = final_color;
	gl_FragData[1] = specular_color;
}


