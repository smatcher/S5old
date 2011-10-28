// glowed_components.vert

void main()
{
	vec3 normal = normalize(gl_NormalMatrix * gl_Normal);

	// NB : according to the OpenGL specification, the light is stored in eye space.
	vec3 light_dir = normalize(vec3(gl_LightSource[0].position - gl_ModelViewMatrix*gl_Vertex));

	// Dot product between the normalized normal and the normalized light direction, which
	// results in the cosine of the angle, clamped to the [0, 1] range.
	float NdotL = max(dot(normal, light_dir), 0.0);

	vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;

	gl_FrontColor = NdotL * diffuse;

	gl_Position = ftransform();
}
