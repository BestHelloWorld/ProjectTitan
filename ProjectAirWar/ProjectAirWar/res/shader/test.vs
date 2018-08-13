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
uniform vec4 U_CameraPos;

varying vec4 V_Texcoord;
varying vec4 V_Normal;
varying vec4 V_WorldPos;

void main()
{
    V_Texcoord = texcoord;
    V_WorldPos = ModelMatrix*position;
    V_Normal = NormalMatrix*normal;
    gl_Position = ProjectMatrix*ViewMatrix*V_WorldPos;
}