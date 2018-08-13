#ifdef GL_ES
precision mediump float;
#endif

#define FOG_DISTANCE 50.0

varying vec4 V_Color;
varying float V_Distance;

float CalcColor(float distance)
{
    return 1.0 - length(gl_PointCoord - vec2(0.5));
}

float CalcFog()
{
    float distance = V_Distance / FOG_DISTANCE;
    return 1.0 - clamp(distance, 0.0, 1.0);
}

void main()
{
    float fog = CalcFog();
    if(fog <= 0.0001) discard;
    float len = CalcColor(V_Distance);
    len = pow(len, 8.0);
    gl_FragColor = V_Color * len * fog;
}