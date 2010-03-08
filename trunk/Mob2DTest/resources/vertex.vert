// #version 110 // What I'm stuck with.

/*
	The following vertex attributes will be made available to shaders:
	
	vec2 m2d_texcoord
	vec3 m2d_vertex
	vec3 m2d_blendcolor
	
	
	More will be made available later... probably.
	I'm stuck with GLSL 1.1. Adapt this to whatever version you are using.
*/

attribute vec2 m2d_texcoord;
attribute vec3 m2d_blendcolor;

varying vec2 texcoord;
varying vec3 blend;

void main(void)
{
	blend = m2d_blendcolor;
	texcoord = m2d_texcoord;
	gl_Position = ftransform();
}

