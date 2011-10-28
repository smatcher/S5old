// shadow_mapping.vert

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;

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
	gl_TexCoord[1] = gl_TextureMatrix[1] * gl_ModelViewMatrix * gl_Vertex;

	gl_Position = ftransform();
}
