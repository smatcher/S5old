// parallax_mapping.frag

uniform sampler2D color_map;
uniform sampler2D normal_map;

// Vecteurs exprimés en tangent space
varying vec3 vertex_to_light_ts;
varying vec3 vertex_to_camera_ts;

// Valeurs utilisées pour la lecture de la hauteur représentée par le canal alpha
// de la normal map
const float PARALLAX_SCALE = 0.08;	// Hauteur maximale du fragment
const float PARALLAX_BIAS = -0.04;	// Biais, utilisé pour ne pas avoir de valeur négative

// Même chose, utilisés pour la lecture de la normale
const float BUMP_HEIGHT_SCALE = 4.0;
const float BUMP_HEIGHT_BIAS = -2.0;

void main()
{
	// Normalisation des vecteurs light et eye
	vec3 light = normalize(vertex_to_light_ts);
	vec3 eye = normalize(vertex_to_camera_ts);

	// On lit une seule fois la normal map au point gl_TexCoord[0].st.
	vec4 original_normal = texture2D(normal_map, gl_TexCoord[0].st);

	// Calcul de la hauteur du fragment, à partir du canal alpha
	// On passe de [0.0, 1.0] à [PARALLAX_BIAS, PARALLAX_SCALE + PARALLAX_BIAS]
	float height = original_normal.a * PARALLAX_SCALE + PARALLAX_BIAS;

	// Perturbation des coordonnées de texture utilisées par la suite
	// On perturbe dans le sens indiqué par eye.xy, et ce d'autant plus que le fragment est haut
	// et que la composante en Z (hauteur) de la normale est importante.
	vec2 new_tex_coords = gl_TexCoord[0].st + height * eye.xy * original_normal.z;

	// Récupération de la normale
	// On passe de [0.0, 1.0] à [-1.0, 1.0]
	vec3 new_normal = texture2D(normal_map, new_tex_coords).xyz * 2.0 - 1.0;

	// Calcul du paramètre ambiant
	vec4 ambient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;

	// Calcul du paramètre diffus
	float diffuse_factor = max(dot(light, new_normal), 0.0);
	vec4 diffuse = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse * diffuse_factor;

	// Calcul du paramètre spéculaire
	vec3 reflected_light = reflect(-light, new_normal);
	float specular_factor = pow(clamp(dot(reflected_light, eye), 0.0, 1.0), gl_FrontMaterial.shininess);
	vec4 specular = gl_LightSource[0].specular * gl_FrontMaterial.specular * specular_factor;

	// Récupération de la couleur indiquée dans la color map
	vec4 color = texture2D(color_map, new_tex_coords);

	gl_FragColor = color*ambient + color*diffuse + specular;
}

