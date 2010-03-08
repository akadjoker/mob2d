// #version 110

// All it does is draw the texture.
uniform sampler2D m2d_texture0;

varying vec2 texcoord;
varying vec3 blend;

void main(void)
{
	vec4 fragcol = texture2D(m2d_texture0, texcoord.st);
	gl_FragColor = fragcol * vec4(blend, 1.0);
}

