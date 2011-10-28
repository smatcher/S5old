// soft_shadow_mapping.vert - même chose que le shader shadow_mapping_self, mais avec un rendu dans
// une autre texture pour qu'ensuite un flou soit appliqué

varying float light_dot_normal;	// Sert à indiquer si la face fait face à la lumière ou pas.
varying vec4 diffuse_color;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;

	vec4 transformed_vertex = gl_ModelViewMatrix * gl_Vertex;

	// On projette chaque vertex dans le light clip space pour pouvoir comparer avec le texel du Z-buffer
	// correspondant.

	// Marche suivie :
	// - multiplication par gl_ModelViewMatrix :
	// object space -> world space (application de la matrice "model")
	// world space -> camera eye space (application de la matrice "camera view")

	// - multiplication par gl_TextureMatrix[1] :
	// camera eye space -> world space (application de camera_view_matrix.getInverse())
	// world space -> light eye space (application de light_view_matrix)
	// light eye space -> light clip space (application de light_projection_matrix)
	// light clip space -> shadow map space (application de bias_matrix : on se ramène de [-1, 1] à [0, 1])
	gl_TexCoord[1] = gl_TextureMatrix[1] * transformed_vertex;

	// Vecteur de lumière
	vec3 light_vector = normalize(vec3(gl_LightSource[0].position - transformed_vertex));

	// Normale à la face
	vec3 normal = gl_NormalMatrix * gl_Normal;

	// Calcul du produit scalaire entre le vecteur de la lumière et la normale
	light_dot_normal = dot(light_vector, normal);

	// Calcul de la valeur diffuse du vertex
	diffuse_color = light_dot_normal * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;

	gl_Position = ftransform();
}
