// water.frag

uniform sampler2D tex_normalmap;
uniform sampler2D tex_dudvmap;
uniform sampler2D tex_reflection;
uniform sampler2D tex_refraction;
uniform sampler2D tex_depth;

uniform vec4 light_position;

varying vec4 position_viewspace;	// = ftransform();
varying vec4 position_cameraspace;	// = gl_ModelViewMatrix*gl_Vertex;
varying vec4 light_direction_ts;	// "ts" : tangent space.
varying vec4 view_direction_ts;	// Vecteur allant du vertex vers la caméra, exprimé en tangent space.

void main()
{
	const float distortion_factor = 0.015;
	const float refraction_factor = 0.009;
	const float specular_exponent = 10.0;	// jusqu'à 128.0 max ; plus il est grand, moins c'est éclairé...(normal ???)
	const vec4 water_color = vec4(0.0, 0.0, 0.2, 1.0);
	const vec4 specular_material_color = vec4(1.0, 1.0, 1.0, 1.0);

	const float depth_bias = -0.4;
	const vec4 depth_bias_vector = vec4(depth_bias, depth_bias, depth_bias, 0.0);

	// Calcul d'un offset, utilisé pour les texture lookups suivants, à partir de la valeur
	// dans la dudvmap.
	// IDEE : mettre un signe - ^^
	//vec4 offset = texture2D(tex_dudvmap, -gl_TexCoord[0].st) * distortion_factor;
	vec4 offset = texture2D(tex_dudvmap, -gl_TexCoord[0].st) * distortion_factor;

	// Récupération du "gradient" (mais avec z=0), dont les valeurs sont ramenées entre -1.0 et 1.0,
	// normalisé et multiplié par un facteur.
	vec4 dudv = texture2D(tex_dudvmap, gl_TexCoord[0].st + vec2(offset));
	dudv = normalize(dudv * 2.0 - 1.0) * refraction_factor;	// NB : sans la normalisation, c'est moche.

	// Récupération de la normale, dont les valeurs sont ramenées entre -1.0 et 1.0.
	vec4 normal = texture2D(tex_normalmap, gl_TexCoord[0].st + vec2(offset));
	normal = normal * 2.0 - 1.0;
	normal.a = 0.0;	// un produit scalaire avec la normale donnera une valeur alpha nulle ; est-ce le but ?
					// Ou alors c'est pour l'utilisation avec reflect() plus loin...

	// Calcul de proj_tex_coords, coordonnées de texture telles qu'un
	// texture lookup dans les textures de réflexion ou de réfraction
	// donne une réflexion ou une réfraction décalées suivant la valeur de dudv

	// - récupération de la position du fragment sur l'écran (entre -1.0 et 1.0)
	vec4 proj_tex_coords = position_viewspace / position_viewspace.q;

	// - on ramène proj_tex_coords entre 0.0 et 1.0
	proj_tex_coords = (proj_tex_coords + 1.0) * 0.5;

	// - on décale suivant dudv
	proj_tex_coords += dudv;

	// - clamp entre 0.001 et 0.999 (pas absolument nécessaire...) ; évite les problèmes de répétition de texture ?
	proj_tex_coords = clamp(proj_tex_coords, 0.001, 0.999);

	// Calcul du fragment de réflexion
	vec4 reflection = texture2D(tex_reflection, proj_tex_coords.xy);

	// Calcul du fragment de réfraction
	vec4 refraction = texture2D(tex_refraction, proj_tex_coords.xy);

	// Calcul du terme de Fresnel et de son complémentaire à 1.
	// Ils permettent d'indiquer quelle est la proportion de rayon réfléchi et de rayon réfracté
	// et sont calculés suivant la position de la caméra et la normale à la surface de l'eau.
	vec4 reflected_light_ts = normalize(reflect(-light_direction_ts, normal));
	vec4 one_minus_Fresnel = vec4(dot(normal, reflected_light_ts));
	vec4 Fresnel = 1.0 - one_minus_Fresnel;

	// Récupération du complémentaire à 1 de la valeur de profondeur
	vec4 depth = texture2D(tex_depth, proj_tex_coords.xy);

	// Bidouille à moi : on "noircit" la depth texture (les objets apparaissent plus près)
	depth = clamp(depth+depth_bias_vector, vec4(0.0), vec4(1.0));

	vec4 one_minus_depth = 1.0 - depth;

	// Calcul de la réflexion spéculaire
	float intensity = clamp(dot(reflected_light_ts, normalize(view_direction_ts)), 0.0, 1.0);
	vec4 specular_factor = vec4(pow(intensity, specular_exponent));
	vec4 specular_color = gl_LightSource[0].specular * specular_material_color * specular_factor;


	gl_FragColor =	refraction*one_minus_Fresnel*one_minus_depth +
					water_color*one_minus_Fresnel*depth +
					reflection*Fresnel +
					specular_color;

/*	vec4 debug_frag_color =	specular_color;

	gl_FragColor =	refraction*one_minus_Fresnel*one_minus_depth*0.001 +
					water_color*one_minus_Fresnel*depth*0.001 +
					reflection*Fresnel*0.001 +
					specular_color*0.001 +
					debug_frag_color;
*/
}
