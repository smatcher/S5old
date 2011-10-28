// water.vert

uniform sampler2D tex_normalmap;
uniform sampler2D tex_dudvmap;
uniform sampler2D tex_reflection;
uniform sampler2D tex_refraction;
uniform sampler2D tex_depth;

uniform vec4 light_position;

varying vec4 position_viewspace;
varying vec4 light_direction_ts;
varying vec4 view_direction_ts;	// Utilisé pour le reflet spéculaire.

void main()
{
	// Pour le passage en tangent space
	vec4 tangent = vec4(1.0, 0.0, 0.0, 0.0);
	vec4 normal = vec4(0.0, 1.0, 0.0, 0.0);
	vec4 bi_normal = vec4(0.0, 0.0, 1.0, 0.0);

	gl_TexCoord[0] = gl_MultiTexCoord0;

	// Vecteur allant du vertex vers la lumière
	vec4 light_direction = light_position - gl_Vertex;

	// Passage de light_direction en tangent space
	light_direction_ts.x = dot(light_direction, tangent);
	light_direction_ts.y = dot(light_direction, bi_normal);
	light_direction_ts.z = dot(light_direction, normal);
	light_direction_ts.w = 1.0;

	// Vecteur indiquant la position du vecteur par rapport à la fenêtre
	vec4 view_direction = -gl_ModelViewMatrix*gl_Vertex;

	// Passage de view_direction en tangent space
	// Utilisé pour le reflet spéculaire.
	// POURQUOI ??? Pourquoi est-ce que si je ne passe PAS en tangent space, alors j'ai un résultat correct, et autrement non ???
	/*view_direction_ts.x = dot(view_direction, tangent);
	view_direction_ts.y = dot(view_direction, normal);
	view_direction_ts.z = dot(view_direction, bi_normal);
	view_direction_ts.w = 1.0;*/

	view_direction_ts = view_direction;
	view_direction_ts.w = 1.0;

	position_viewspace = ftransform();

	gl_Position = position_viewspace;
}
