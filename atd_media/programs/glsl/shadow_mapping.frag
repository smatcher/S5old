// shadow_mapping.frag

uniform sampler2D tex;
uniform sampler2DShadow tex_depth;
/*
void main()
{
	// Récupération de la texture "normale"
	vec4 texel = texture2D(tex, gl_TexCoord[0].st);

	vec2 tex_1_proj = gl_TexCoord[1].st / gl_TexCoord[1].q;

	// Si on est en dehors de la shadow map, on est éclairé
	if(tex_1_proj.s < 0.0 || tex_1_proj.s > 1.0 || tex_1_proj.t < 0.0 || tex_1_proj.t > 1.0)
	{
		gl_FragColor = texel;
	}
	else
	{
		// Valeur lue dans la shadow map
		float shadow_map_value = texture2DProj(tex_depth, gl_TexCoord[1]).r;

		// Distance entre le fragment et la source de lumière
		float dist_to_light = length(gl_LightSource[0].position - gl_FragCoord);

		//if(dist_to_light > shadow_map_value)
		//{
		//	gl_FragColor = texel*0.5;
		//}
		//else
		//{
		//	gl_FragColor = texel;
		//}

		gl_FragColor = vec4(dist_to_light) + texel*0.001;

		// NB : le test sur gl_TexCoord[1].q permet d'éviter la reverse projection.
		//if(shadow.r <= 0.0 && gl_TexCoord[1].q >= 0.0)
		//	gl_FragColor = texel*0.5;	// Ombre
		//else
		//	gl_FragColor = texel;	// Lumière

		//gl_FragColor = shadow + texel*0.001;

		//gl_FragColor = vec4(shadow_map_value) + texel*0.001;
	}
}
*/

void main()
{
	vec4 shadow;

	vec4 texel = texture2D(tex, gl_TexCoord[0].st);

	vec2 tex_1_proj = gl_TexCoord[1].st / gl_TexCoord[1].q;

	if(tex_1_proj.s < 0.0 || tex_1_proj.s > 1.0 || tex_1_proj.t < 0.0 || tex_1_proj.t > 1.0)
	{
		gl_FragColor = texel*0.5;
	}
	else
	{

		shadow = shadow2DProj(tex_depth, gl_TexCoord[1]);

		// NB : le test sur gl_TexCoord[1].q permet d'éviter la reverse projection.
		if(shadow.r <= 0.0 && gl_TexCoord[1].q >= 0.0)
			gl_FragColor = texel*0.5;	// Ombre
		else
			gl_FragColor = texel;	// Lumière

	//	gl_FragColor = shadow + texel*0.001;
	}
}

