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

#if defined LIGHT_SPOT || defined LIGHT_SUN
	varying vec3 eyespotdir;
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

	#ifdef LIGHT_SUN
		vec3 lightvec = eyespotdir;
	#else
		vec3 lightvec = eyelightpos - eyepos.xyz;
	#endif
	vec3 viewvec = normalize(eyepos.rgb);
	vec3 normal = 2.0*texture2D(gbuffer_normal, screen_pos).rgb-1.0;
	vec4 diffuse = texture2D(gbuffer_diffuse, screen_pos);	// rgb:diffuse a:sky (if 0)
	vec4 specular = texture2D(gbuffer_specular, screen_pos); // rgb:specularity a:shininess/128.0

	#ifdef LIGHT_SUN
		float attenuation = 1.0;
	#else
		float dist = length(lightvec);
		float attenuation = 1.0/ (
				gl_LightSource[0].constantAttenuation +
				gl_LightSource[0].linearAttenuation*dist +
				gl_LightSource[0].quadraticAttenuation*dist*dist
		);
	#endif

	#ifdef SHADOW_MAP
		attenuation *= texture2D(shadowmap, screen_pos).r;
	#endif

	lightvec = normalize(lightvec);
	vec3 halfvec = normalize(lightvec - viewvec);
	vec3 idiff = clamp(dot(lightvec, normal) * gl_LightSource[0].diffuse.rgb * diffuse.rgb, 0.0, 1.0);
	vec3 ispec = pow(clamp(dot(halfvec,normal),0.0,1.0),128.0*specular.a) * gl_LightSource[0].specular.rgb * specular.rgb;

	#ifdef LIGHT_SPOT
		float spotAngle = dot(lightvec,eyespotdir);
		if(spotAngle < gl_LightSource[0].spotCutoff)
			attenuation = 0.0;
	#endif


	gl_FragColor = vec4((idiff+ispec) * attenuation * diffuse.a, 1.0);
}
