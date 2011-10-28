// fur.vert

uniform float layer;
uniform float fur_lenght;
uniform float fur_repetition;
//attribute vec3 normal;

varying vec4 diffuse_color;

void main()
{
	//gl_TexCoord[0]=ftransform();
	gl_TexCoord[0]=gl_MultiTexCoord0;
	gl_TexCoord[1]=gl_MultiTexCoord0*fur_repetition;
	//gl_TexCoord[1]=gl_MultiTexCoord1*0.0001+ftransform();
	//gl_TexCoord[2]=ftransform();
	gl_TexCoord[2]=gl_MultiTexCoord0;

	gl_Position = gl_ModelViewProjectionMatrix * (gl_Vertex+vec4(gl_Normal,0)*layer*fur_lenght);

	vec4 transformed_vertex = gl_ModelViewMatrix * gl_Vertex;

	// Vecteur de lumière
	vec3 light_vector = normalize(vec3(gl_LightSource[0].position - transformed_vertex));

	// Normale à la face
	vec3 normal = gl_NormalMatrix * gl_Normal;

	// Calcul de la valeur diffuse du vertex
	diffuse_color = dot(light_vector, normal) * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
}

