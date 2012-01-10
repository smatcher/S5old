// bump_mapping.vert
// Remarques :
// 1) ne serait-ce pas plutôt la modelview qu'il faudrait utiliser pour calculer le vecteur t ?
// 2) on ne multiplie par la position de la lumière par la normal matrix, car l'on considère qu'elle
//    est donnée en world coordinates.

/*
void main()
{
	gl_Position = ftransform();
}
*/


// Vecteurs exprimés en tangent space
varying vec3 vertex_to_light_ts;
varying vec3 vertex_to_camera_ts;

attribute vec3 tangent;

void main()
{
	gl_TexCoord[0] = gl_MultiTexCoord0;

	// Calcul des vecteurs N, T et B (Normale, Tangente, Bitangente), exprimés dans l'espace de la caméra
	// NB : pour un programme appelant bien fait, pas besoin de normalize() !
	vec3 n = normalize(gl_NormalMatrix * gl_Normal);
	vec3 t = normalize(gl_NormalMatrix * tangent);
	vec3 b = cross(n, t);

	// Calcul de la position du vertex exprimé dans l'espace de la caméra
	vec3 vertex_pos = vec3(gl_ModelViewMatrix * gl_Vertex);

	// Calcul du vecteur allant du vertex vers la lumière, dans l'espace de la caméra
	vec3 vertex_to_light = gl_LightSource[0].position.xyz - vertex_pos;

	// On le passe en tangent space
	vertex_to_light_ts.x = dot(vertex_to_light, t);	// x = projection sur la tangente
	vertex_to_light_ts.y = dot(vertex_to_light, b);	// y = projection sur la bitangente
	vertex_to_light_ts.z = dot(vertex_to_light, n);	// z = projection sur la normale

	// Calcul du vecteur allant du vertex vers la caméra (eye vector), dans l'espace de la caméra
	vec3 vertex_to_camera = -vertex_pos;

	// On le passe en tangent space
	vertex_to_camera_ts.x = dot(vertex_to_camera, t);
	vertex_to_camera_ts.y = dot(vertex_to_camera, b);
	vertex_to_camera_ts.z = dot(vertex_to_camera, n);

	gl_Position = ftransform();
}

