uniform sampler2D color_map, map0, map1, map2, map3, map4, map5;
varying vec4 shadowcoord0, shadowcoord1, shadowcoord2, shadowcoord3, shadowcoord4, shadowcoord5;

void main (void)
{
	vec4 shadowCoordinateWdivide0 = shadowcoord0/shadowcoord0.w;
	vec4 shadowCoordinateWdivide1 = shadowcoord1/shadowcoord1.w;
	vec4 shadowCoordinateWdivide2 = shadowcoord2/shadowcoord2.w;
	vec4 shadowCoordinateWdivide3 = shadowcoord3/shadowcoord3.w;
	vec4 shadowCoordinateWdivide4 = shadowcoord4/shadowcoord4.w;
	vec4 shadowCoordinateWdivide5 = shadowcoord5/shadowcoord5.w;
	shadowCoordinateWdivide0.z += 0.0005;
	shadowCoordinateWdivide1.z += 0.0005;
	shadowCoordinateWdivide2.z += 0.0005;
	shadowCoordinateWdivide3.z += 0.0005;
	shadowCoordinateWdivide4.z += 0.0005;
	shadowCoordinateWdivide5.z += 0.0005;

	vec4 tex_color = texture2D(color_map, gl_TexCoord[0].st);
	vec4 color = vec4(0,0,0,1);

	float distanceFromLight = 0;
	if(shadowCoordinateWdivide0.s >= 0
		&& shadowCoordinateWdivide0.s <=1
		&& shadowCoordinateWdivide0.t >= 0
		&& shadowCoordinateWdivide0.t <= 1) {
		distanceFromLight = texture2D(map0, shadowCoordinateWdivide0.st).z;
		color = vec4(0,1,0,1);
	}

	if(shadowcoord0.w < 0.0) {
		color = vec4(1,0,0,1);
	}
	if(shadowcoord0.w > 0.0 && distanceFromLight < shadowCoordinateWdivide0.z) {
		color = vec4(0,0,1,1);
	}

	gl_FragColor = 0.5f * (color + tex_color);
}

