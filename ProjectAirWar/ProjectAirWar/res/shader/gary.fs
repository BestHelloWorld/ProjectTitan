#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D U_Texture;

varying vec2 V_Texcoord;

void main()
{
	vec4 color = texture2D(U_Texture, V_Texcoord);
	float gary = (color.r + color.g + color.b) / 3.0;
	gl_FragColor = vec4(gary, gary, gary, 1.0);
}
