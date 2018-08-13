#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D U_Texture;
uniform vec4 U_Color;

varying vec2 V_Texcoord;

void main()
{
    gl_FragColor = texture2D(U_Texture, V_Texcoord) * U_Color;
}