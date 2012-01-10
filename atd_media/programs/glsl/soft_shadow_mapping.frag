// soft_shadow_mapping.frag - même chose que le shader shadow_mapping_self, mais avec un rendu dans
// une autre texture pour qu'ensuite un flou soit appliqué

uniform sampler2D tex;
uniform sampler2DShadow tex_depth;
uniform float offset_x;
uniform float offset_y;
varying float light_dot_normal;
varying vec4 diffuse_color;

void main()
{
	vec4 shadow;

	// Pour le "soft" :
	vec4 shadow_up;
	vec4 shadow_down;
	vec4 shadow_left;
	vec4 shadow_right;

	vec4 texel = texture2D(tex, gl_TexCoord[0].st);

	vec4 not_shadowed_color = diffuse_color * texel;

	vec2 tex_1_proj = gl_TexCoord[1].st / gl_TexCoord[1].q;

	// Si on n'est pas dans le champ de vision de la lumière, on est ombré.
	if(tex_1_proj.s < 0.0 || tex_1_proj.s > 1.0 || tex_1_proj.t < 0.0 || tex_1_proj.t > 1.0)
	{
		gl_FragColor = not_shadowed_color*0.5;
	}
	// En fonction de combien la face fait face à la lumière, on l'ombre plus ou moins
	else if(light_dot_normal <= 0.05)
	{
		gl_FragColor = not_shadowed_color*0.5;
	}
	else if(light_dot_normal <= 0.1)
	{
		gl_FragColor = not_shadowed_color*0.6;
	}
	else if(light_dot_normal <= 0.15)
	{
		gl_FragColor = not_shadowed_color*0.7;
	}
	else if(light_dot_normal <= 0.2)	// Si la face ne fait pas face à la lumière, elle est ombrée
	{
		gl_FragColor = not_shadowed_color*0.8;
	}
	// Si la face fait trop face à la lumière, on utilise la shadow map pour décider si elle doit être éclairée ou pas.
	else
	{
		shadow = shadow2DProj(tex_depth, gl_TexCoord[1]);

		shadow_up =		shadow2DProj(tex_depth, gl_TexCoord[1] + vec4(0.0, offset_y, 0.0, 0.0));
		shadow_down =	shadow2DProj(tex_depth, gl_TexCoord[1] + vec4(0.0, -offset_y, 0.0, 0.0));
		shadow_left =	shadow2DProj(tex_depth, gl_TexCoord[1] + vec4(-offset_x, 0.0, 0.0, 0.0));
		shadow_right =	shadow2DProj(tex_depth, gl_TexCoord[1] + vec4(offset_x, 0.0, 0.0, 0.0));

		// NB : le test sur gl_TexCoord[1].q permet d'éviter la reverse projection.
		/*if(shadow.r <= 0.0 && gl_TexCoord[1].q >= 0.0)
		{
			gl_FragColor = not_shadowed_color*0.5;	// Ombre
		}*/

		if(shadow.r <= 0.0 && gl_TexCoord[1].q >= 0.0)
		{
			//float shadow_sum = 0.5;
			float shadow_sum = 1.0;

			if(shadow_up.r > 0.0)
				//shadow_sum -= 0.1;
				shadow_sum -= 0.5;

/*			if(shadow_down.r > 0.0)
				shadow_sum -= 0.1;

			if(shadow_left.r > 0.0)
				shadow_sum -= 0.1;

			if(shadow_right.r > 0.0)
				shadow_sum -= 0.1;
*/

			gl_FragColor = shadow_sum * not_shadowed_color;
		}
		else
			gl_FragColor = not_shadowed_color;	// Lumière
	}
}
