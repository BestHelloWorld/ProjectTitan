#ifdef GL_ES
precision mediump float;
#endif

attribute vec4 position;
attribute vec4 texcoord;
attribute vec4 normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectMatrix;

uniform mat4 NormalMatrix;

varying vec3 V_Normal;
varying vec4 V_WorldPos;

void main()
{
	V_Normal = (NormalMatrix*normal).xyz;
	V_WorldPos = ModelMatrix*position;
	gl_Position = ProjectMatrix*ViewMatrix*V_WorldPos;
}