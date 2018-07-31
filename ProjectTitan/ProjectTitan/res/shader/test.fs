#ifdef GL_ES
precision mediump float;
#endif

uniform vec4 U_LightPos;
uniform vec4 U_CameraPos;
uniform sampler2D U_Texture;

varying vec4 V_Texcoord;
varying vec4 V_Normal;
varying vec4 V_WorldPos;

void main()
{
    vec3 L = normalize(U_LightPos.xyz);
    vec3 N = normalize(V_Normal.xyz);

    float diffuse = max(0.0, dot(L, N)) * 0.3;
    float specular;
    if(diffuse > 0.0)
    {
        vec3 worldPos = V_WorldPos.xyz;
        vec3 viewDir = normalize(U_CameraPos.xyz - worldPos);
        vec3 halfL = normalize(L+viewDir);
        specular = pow(max(0.0, dot(halfL, N)), 16.0) * 0.7;
    }
    gl_FragColor = texture2D(U_Texture, V_Texcoord.xy) + diffuse + specular;
}