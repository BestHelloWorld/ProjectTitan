#ifdef GL_ES
precision mediump float;
#endif

attribute vec4 position;
attribute vec4 texcoord;

varying vec2 V_Position;
varying vec2 V_Texcoord;

void main()
{
	V_Texcoord = texcoord.xy;
	V_Position = position.xy;
	gl_Position = position;
}