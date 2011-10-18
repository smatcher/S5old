uniform sampler2D shadowmap;
uniform vec2 screen_size;
varying vec3 normal, lightDir, eyeVec;

void main (void)
{
	vec4 color = texture2D(shadowmap, gl_FragCoord.st/screen_size);

	vec4 final_color = color * (
	(gl_FrontLightModelProduct.sceneColor * gl_FrontMaterial.ambient) + 
	(gl_LightSource[0].ambient * gl_FrontMaterial.ambient));
							
//	vec3 N = normalize(normal);
//	vec3 L = normalize(lightDir);
	
//	float lambertTerm = dot(N,L) * shadow.r;
	float lambertTerm = dot(normal,lightDir) * color.r;
	
	if(lambertTerm > 0.0)
	{
		final_color += color * (
							gl_LightSource[0].diffuse * 
		               gl_FrontMaterial.diffuse * 
					   lambertTerm);	
	}

	gl_FragColor = final_color;			
}

