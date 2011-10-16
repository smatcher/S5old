uniform sampler2D shadow_map, map0, map1, map2, map3, map4, map5;
uniform vec2 screen_size;
varying vec4 shadowcoord0, shadowcoord1, shadowcoord2, shadowcoord3, shadowcoord4, shadowcoord5;

void main (void)
{
	vec4 shadcoord_wdiv0 = shadowcoord0/shadowcoord0.w;
	vec4 shadcoord_wdiv1 = shadowcoord1/shadowcoord1.w;
	vec4 shadcoord_wdiv2 = shadowcoord2/shadowcoord2.w;
	vec4 shadcoord_wdiv3 = shadowcoord3/shadowcoord3.w;
	vec4 shadcoord_wdiv4 = shadowcoord4/shadowcoord4.w;
	vec4 shadcoord_wdiv5 = shadowcoord5/shadowcoord5.w;
	shadcoord_wdiv0.z += 0.0005;
	shadcoord_wdiv1.z += 0.0005;
	shadcoord_wdiv2.z += 0.0005;
	shadcoord_wdiv3.z += 0.0005;
	shadcoord_wdiv4.z += 0.0005;
	shadcoord_wdiv5.z += 0.0005;

	vec4 shadow_color = texture2D(shadow_map, gl_FragCoord.st / screen_size);
	vec4 color = vec4(0,0,0,1);

	float distanceFromLight = 0;
	float distanceFromLight2 = 0;
	if(   shadowcoord0.w > 0.0 
		&& shadcoord_wdiv0.s >= 0
		&& shadcoord_wdiv0.s <=1
		&& shadcoord_wdiv0.t >= 0
		&& shadcoord_wdiv0.t <= 1) {
		distanceFromLight = texture2D(map0, shadcoord_wdiv0.st).z;
		distanceFromLight2 = shadcoord_wdiv0.z;
		color = vec4(1,1,1,1);
	}
	if(   shadowcoord1.w > 0.0 
		&& shadcoord_wdiv1.s >= 0
		&& shadcoord_wdiv1.s <=1
		&& shadcoord_wdiv1.t >= 0
		&& shadcoord_wdiv1.t <= 1) {
		distanceFromLight = texture2D(map1, shadcoord_wdiv1.st).z;
		distanceFromLight2 = shadcoord_wdiv1.z;
		color = vec4(1,1,1,1);
	}
	if(   shadowcoord2.w > 0.0 
		&& shadcoord_wdiv2.s >= 0
		&& shadcoord_wdiv2.s <=1
		&& shadcoord_wdiv2.t >= 0
		&& shadcoord_wdiv2.t <= 1) {
		distanceFromLight = texture2D(map2, shadcoord_wdiv2.st).z;
		distanceFromLight2 = shadcoord_wdiv2.z;
		color = vec4(1,1,1,1);
	}
	if(   shadowcoord3.w > 0.0 
		&& shadcoord_wdiv3.s >= 0
		&& shadcoord_wdiv3.s <=1
		&& shadcoord_wdiv3.t >= 0
		&& shadcoord_wdiv3.t <= 1) {
		distanceFromLight = texture2D(map3, shadcoord_wdiv3.st).z;
		distanceFromLight2 = shadcoord_wdiv3.z;
		color = vec4(1,1,1,1);
	}
	if(   shadowcoord4.w > 0.0 
		&& shadcoord_wdiv4.s >= 0
		&& shadcoord_wdiv4.s <=1
		&& shadcoord_wdiv4.t >= 0
		&& shadcoord_wdiv4.t <= 1) {
		distanceFromLight = texture2D(map4, shadcoord_wdiv4.st).z;
		distanceFromLight2 = shadcoord_wdiv4.z;
		color = vec4(1,1,1,1);
	}
	if(   shadowcoord5.w > 0.0 
		&& shadcoord_wdiv5.s >= 0
		&& shadcoord_wdiv5.s <=1
		&& shadcoord_wdiv5.t >= 0
		&& shadcoord_wdiv5.t <= 1) {
		distanceFromLight = texture2D(map5, shadcoord_wdiv5.st).z;
		distanceFromLight2 = shadcoord_wdiv5.z;
		color = vec4(1,1,1,1);
	}
	if(distanceFromLight < distanceFromLight2) {
		color = vec4(0,0,0,1);
	}

	gl_FragColor = clamp(color + shadow_color, vec4(0,0,0,1), vec4(1,1,1,1));
}

