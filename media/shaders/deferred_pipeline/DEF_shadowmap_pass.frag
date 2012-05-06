uniform sampler2D eye_depth, light_depth0;

uniform mat4 inverse_projection;
uniform mat4 inverse_view;

#ifdef LIGHT_OMNI
	uniform mat4 texture_matrices0[6];
#else
	uniform mat4 texture_matrix0;
#endif
/*
#ifdef LIGHT_OMNI
	uniform mat4 texture_matrix1;
	uniform mat4 texture_matrix2;
	uniform mat4 texture_matrix3;
	uniform mat4 texture_matrix4;
	uniform mat4 texture_matrix5;
#endif
*/

varying vec2 screen_pos;

void main()
{
	vec4 depthsample = texture2D(eye_depth, screen_pos);
	float depth = depthsample.x * 255.0/256.0 +
						depthsample.y * 255.0/65536.0 +
						depthsample.z * 255.0/16777216.0;
	vec4 scpos = vec4(screen_pos, depth, 1.0) * 2.0 - 1.0;
	vec4 eyepos = inverse_projection * scpos;
	vec4 worldpos = inverse_view * eyepos;

#ifdef LIGHT_OMNI
	vec4 shadowcoord0 = texture_matrices0[0] * worldpos;
	shadowcoord0.xyz = shadowcoord0.xyz/shadowcoord0.w;
	shadowcoord0.z += 0.0005;

	vec4 shadowcoord1 = texture_matrices0[1] * worldpos;
	shadowcoord1.xyz = shadowcoord1.xyz/shadowcoord1.w;
	shadowcoord1.z += 0.0005;

	vec4 shadowcoord2 = texture_matrices0[2] * worldpos;
	shadowcoord2.xyz = shadowcoord2.xyz/shadowcoord2.w;
	shadowcoord2.z += 0.0005;

	vec4 shadowcoord3 = texture_matrices0[3] * worldpos;
	shadowcoord3.xyz = shadowcoord3.xyz/shadowcoord3.w;
	shadowcoord3.z += 0.0005;

	vec4 shadowcoord4 = texture_matrices0[4] * worldpos;
	shadowcoord4.xyz = shadowcoord4.xyz/shadowcoord4.w;
	shadowcoord4.z += 0.0005;

	vec4 shadowcoord5 = texture_matrices0[5] * worldpos;
	shadowcoord5.xyz = shadowcoord5.xyz/shadowcoord5.w;
	shadowcoord5.z += 0.0005;
#else
	vec4 shadowcoord0 = texture_matrix0 * worldpos;
	shadowcoord0.xyz = shadowcoord0.xyz/shadowcoord0.w;
	shadowcoord0.z += 0.0005;
#endif

#ifdef LIGHT_OMNI
	vec2 tile = vec2(1.0/3.0, 1.0/2.0);
	float sampdtl = texture2D(light_depth0, shadowcoord0.st*tile).z;
	float fragdtl = shadowcoord0.z;
	float lit = float(
				   shadowcoord0.w > 0.0
				&& shadowcoord0.s >= 0.0
				&& shadowcoord0.s <=1.0
				&& shadowcoord0.t >= 0.0
				&& shadowcoord0.t <= 1.0
				&& sampdtl > fragdtl);

	sampdtl = texture2D(light_depth0, (shadowcoord1.st + vec2(1.0,0.0))*tile).z;
	fragdtl = shadowcoord1.z;
	lit += float(
		   shadowcoord1.w > 0.0
		&& shadowcoord1.s >= 0.0
		&& shadowcoord1.s <=1.0
		&& shadowcoord1.t >= 0.0
		&& shadowcoord1.t <= 1.0
		&& sampdtl > fragdtl);

	sampdtl = texture2D(light_depth0, (shadowcoord2.st + vec2(2.0,0.0))*tile).z;
	fragdtl = shadowcoord2.z;
	lit += float(
		   shadowcoord2.w > 0.0
		&& shadowcoord2.s >= 0.0
		&& shadowcoord2.s <=1.0
		&& shadowcoord2.t >= 0.0
		&& shadowcoord2.t <= 1.0
		&& sampdtl > fragdtl);

	sampdtl = texture2D(light_depth0, (shadowcoord3.st + vec2(0.0,1.0))*tile).z;
	fragdtl = shadowcoord3.z;
	lit += float(
		   shadowcoord3.w > 0.0
		&& shadowcoord3.s >= 0.0
		&& shadowcoord3.s <=1.0
		&& shadowcoord3.t >= 0.0
		&& shadowcoord3.t <= 1.0
		&& sampdtl > fragdtl);

	sampdtl = texture2D(light_depth0, (shadowcoord4.st + vec2(1.0,1.0))*tile).z;
	fragdtl = shadowcoord4.z;
	lit += float(
		   shadowcoord4.w > 0.0
		&& shadowcoord4.s >= 0.0
		&& shadowcoord4.s <=1.0
		&& shadowcoord4.t >= 0.0
		&& shadowcoord4.t <= 1.0
		&& sampdtl > fragdtl);

	sampdtl = texture2D(light_depth0, (shadowcoord5.st + vec2(2.0,1.0))*tile).z;
	fragdtl = shadowcoord5.z;
	lit += float(
		   shadowcoord5.w > 0.0
		&& shadowcoord5.s >= 0.0
		&& shadowcoord5.s <=1.0
		&& shadowcoord5.t >= 0.0
		&& shadowcoord5.t <= 1.0
		&& sampdtl > fragdtl);
#else
	float sampdtl = texture2D(light_depth0, shadowcoord0.st).z;
	float fragdtl = shadowcoord0.z;
	float lit = float(
				   shadowcoord0.w > 0.0
				&& shadowcoord0.s >= 0.0
				&& shadowcoord0.s <=1.0
				&& shadowcoord0.t >= 0.0
				&& shadowcoord0.t <= 1.0
				&& sampdtl > fragdtl);
#endif

	lit = clamp(lit, 0.0, 1.0);
	gl_FragColor = vec4(lit,lit,lit,1.0);
}
