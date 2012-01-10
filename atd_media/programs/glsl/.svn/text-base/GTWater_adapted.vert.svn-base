// GTWater_adapted.vert

varying vec4 refrCoords;
varying vec4 normCoords;
varying vec4 viewCoords;
varying vec4 viewTangetSpace;
varying vec4 lightTangetSpace;

uniform vec4 lightPos/*, cameraPos*/;

void main()
{
	// Because we have a flat plane for water we already know the vectors for tangent space
	vec4 tangent = vec4(1.0, 0.0, 0.0, 0.0);
	vec4 normal = vec4(0.0, 1.0, 0.0, 0.0);
	vec4 biTangent = vec4(0.0, 0.0, 1.0, 0.0);

	// Calculate the vector coming from the vertex to the camera
	//vec4 viewDir = cameraPos - gl_Vertex;
	vec4 viewDir = normalize(-gl_ModelViewMatrix*gl_Vertex);

	// Compute tangent space for the view direction
	// INUTILE, non?
	viewTangetSpace.x = dot(viewDir, tangent);	// Projection de viewDir sur l'abscisse du repère du tangent space
	viewTangetSpace.y = dot(viewDir, biTangent);	// pareil pour y
	viewTangetSpace.z = dot(viewDir, normal);		// et pour z
	viewTangetSpace.w = 1.0;

	// Calculate the vector that the light hits the vertex
	vec4 lightDir = lightPos - gl_Vertex;

	// Compute tangent space for the light direction
	// INUTILE aussi, non?
	lightTangetSpace.x = dot(lightDir, tangent);
	lightTangetSpace.y = dot(lightDir, biTangent);
	lightTangetSpace.z = dot(lightDir, normal);
	lightTangetSpace.w = 1.0;

	//refrCoords = gl_MultiTexCoord1;
	//normCoords = gl_MultiTexCoord2;
	refrCoords = gl_MultiTexCoord0;
	normCoords = gl_MultiTexCoord0;

	// This calculates our current projection coordinates
	viewCoords = gl_ModelViewProjectionMatrix * gl_Vertex;

	gl_Position = viewCoords;
}

