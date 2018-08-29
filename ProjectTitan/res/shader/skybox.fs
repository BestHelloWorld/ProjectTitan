#ifdef GL_ES
precision mediump float;
#endif

uniform samplerCube U_Texture;
uniform vec4 U_CameraPos;
uniform vec4 U_Color;

varying vec4 V_Texcoord;

void main()
{
	vec4 pos = -V_Texcoord;// - U_CameraPos;
	gl_FragColor = textureCube(U_Texture, pos.xyz) * U_Color;
}