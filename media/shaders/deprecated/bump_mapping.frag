// bump_mapping.frag

uniform sampler2D color_map;
uniform sampler2D normal_map;

// Vecteurs exprimés en tangent space
varying vec3 vertex_to_light_ts;
varying vec3 vertex_to_camera_ts;

void main()
{
	// Normalisation des vecteurs light et eye
	vec3 light = normalize(vertex_to_light_ts);
	vec3 eye = normalize(vertex_to_camera_ts);

	// Récupération de la couleur indiquée dans la color map
	vec4 color = texture2D(color_map, gl_TexCoord[0].st);

	// Récupération de la normale (on passe de [0.0, 1.0] à [-1.0, 1.0])
	vec3 normal = texture2D(normal_map, gl_TexCoord[0].st).xyz * 2.0 - 1.0;

	// Calcul du paramètre ambiant
	vec4 ambient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;

	// Calcul du paramètre diffus
	float diffuse_factor = max(dot(light, normal), 0.0);
	vec4 diffuse = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * diffuse_factor;

	// Calcul du paramètre spéculaire
	vec3 reflected_light = reflect(-light, normal);
	float specular_factor = pow(clamp(dot(reflected_light, eye), 0.0, 1.0), gl_FrontMaterial.shininess);
	vec4 specular = gl_LightSource[0].specular * gl_FrontMaterial.specular * specular_factor;

	gl_FragColor = color*ambient + color*diffuse + specular*diffuse_factor;
}

