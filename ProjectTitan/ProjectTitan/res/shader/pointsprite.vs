#ifdef GL_ES
precision mediump float;
#endif

attribute vec4 position;
attribute vec4 color;

uniform mat4 ViewMatrix;
uniform mat4 ProjectMatrix;

varying vec4 V_Color;
varying float V_Distance;

void main()
{
    V_Color = color;
    vec4 ViewPos = ViewMatrix*position;
    gl_Position = ProjectMatrix*ViewPos;
    V_Distance = gl_Position.z;
}