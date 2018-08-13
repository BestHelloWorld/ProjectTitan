#ifdef GL_ES
precision mediump float;
#endif

uniform samplerCube U_Texture;
uniform vec4 U_CameraPos;

varying vec4 V_Texcoord;

void main()
{
	vec4 pos = -V_Texcoord;// - U_CameraPos;
	gl_FragColor = textureCube(U_Texture, pos.xyz);
	// gl_FragColor = vec4(V_Texcoord.x, V_Texcoord.y, 1.0, 1.0);
}