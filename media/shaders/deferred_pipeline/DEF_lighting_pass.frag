uniform sampler2D gbuffer_normal;
uniform sampler2D gbuffer_diffuse;
uniform sampler2D gbuffer_specular;
uniform sampler2D gbuffer_depth;
varying vec2 screen_pos;
varying vec3 eyelightpos;
uniform mat4 inverse_projection;
uniform mat4 inverse_modelview;

#ifdef SHADOW_MAP
	uniform sampler2D shadowmap;
#endif

void main()
{
	vec4 depthsample = texture2D(gbuffer_depth, screen_pos);
	float depth = depthsample.x * 255.0/256.0 +
						depthsample.y * 255.0/65536.0 +
						depthsample.z * 255.0/16777216.0;
	vec4 scpos = vec4(screen_pos, depth, 1.0) * 2.0 - 1.0;

	vec4 eyepos = inverse_projection * scpos;
	eyepos = eyepos/eyepos.w;

	vec3 lightvec = eyelightpos - eyepos.xyz;
	vec3 viewvec = normalize(eyepos.rgb);
	vec3 halfvec = normalize(lightvec + viewvec);
	vec3 normal = texture2D(gbuffer_normal, screen_pos).rgb;
	vec4 diffuse = texture2D(gbuffer_diffuse, screen_pos);	// rgb:diffuse a:sky (if 0)
	vec4 specular = texture2D(gbuffer_specular, screen_pos); // rgb:specularity a:shininess

	float attenuation = clamp(1.0 - length(lightvec/5.0), 0.0, 1.0);

	#ifdef SHADOW_MAP
		attenuation *= texture2D(shadowmap, screen_pos).r;
	#endif

	lightvec = normalize(lightvec);
	vec3 idiff = clamp(dot(lightvec, normal)*diffuse.rgb, 0.0, 1.0);
	vec3 ispec = pow(clamp(dot(halfvec,normal),0.0,1.0),specular.a) * specular.rgb;

	#ifdef BLOOM
		gl_FragData[0] = vec4((idiff+ispec) * attenuation * diffuse.a, 1.0);
		gl_FragData[1] = vec4((ispec) * attenuation * diffuse.a , 1.0);
	#else
		gl_FragColor = vec4((idiff+ispec) * attenuation * diffuse.a, 1.0);
	#endif
}
