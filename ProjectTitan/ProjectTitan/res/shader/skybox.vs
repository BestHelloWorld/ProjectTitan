#ifdef GL_ES
precision mediump float;
#endif
attribute vec4 position;
attribute vec4 texcoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectMatrix;

varying vec4 V_Texcoord;

void main()
{
	V_Texcoord = -position;
	gl_Position = ProjectMatrix*ViewMatrix*ModelMatrix*position;
}