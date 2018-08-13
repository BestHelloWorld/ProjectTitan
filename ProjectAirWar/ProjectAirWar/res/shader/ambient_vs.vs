#ifdef GL_ES
precision mediump float;
#endif

attribute vec4 position;
attribute vec4 texcoord;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectMatrix;

uniform vec4 V_AmbientMaterial;
uniform vec4 V_AmbientColor;

varying vec4 V_Color;

void main()
{

	V_Color = V_AmbientMaterial*V_AmbientColor;
	gl_Position = ProjectMatrix*ViewMatrix*ModelMatrix*position;
}