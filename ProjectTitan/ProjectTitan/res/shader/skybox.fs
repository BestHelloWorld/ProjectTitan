#ifdef GL_ES
precision mediump float;
#endif

uniform samplerCube U_Texture;
varying vec4 V_Texcoord;

void main()
{
	gl_FragColor = textureCube(U_Texture, normalize(V_Texcoord.xyz));
}