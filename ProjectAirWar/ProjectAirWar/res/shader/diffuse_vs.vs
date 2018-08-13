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

uniform vec4 U_LightPos;

varying vec4 V_Color;

void main()
{
	vec3 L = U_LightPos.xyz;
	L = normalize(L);
	vec3 N = (ModelMatrix*normal).xyz;
	vec4 diffuse = vec4(1.0)*max(0.0, dot(L, N));

	V_Color = diffuse;
	gl_Position = ProjectMatrix*ViewMatrix*ModelMatrix*position;
}