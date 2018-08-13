#ifdef GL_ES
precision mediump float;
#endif

uniform vec4 U_LightPos;
uniform vec4 U_CameraPos;
uniform vec4 U_FogOption;

uniform samplerCube U_Texture;

varying vec4 V_Texcoord;
varying vec4 V_Normal;
varying vec4 V_WorldPos;
varying vec4 V_ViewPos;

float CalcFog(float distance)
{
    float alpha = (distance) / (U_FogOption.y - U_FogOption.x);
    alpha = 1.0 - clamp(alpha, 0.0, 1.0);
    return alpha;
}

vec4 TextureCube()
{
    vec4 viewDir = normalize(V_WorldPos-U_CameraPos);
    return textureCube(U_Texture, -normalize(reflect(viewDir, V_Normal)).xyz);
}

void main()
{
    vec3 L = normalize(U_LightPos.xyz);
    vec3 N = normalize(V_Normal.xyz);

    float diffuse = max(0.0, dot(L, N));
    float specular;
    if(diffuse > 0.0)
    {
        vec3 worldPos = V_WorldPos.xyz;
        vec3 viewDir = normalize(U_CameraPos.xyz - worldPos);
        vec3 halfL = normalize(L+viewDir);
        specular = pow(max(0.0, dot(halfL, N)), 16.0);
        // vec3 ref = reflect(-L, N);
        // vec3 viewDir = normalize(U_CameraPos.xyz - V_WorldPos.xyz);
        // specular = pow(max(0.0, dot(ref, -viewDir)), 4.0);
    }
    float alpha = CalcFog(abs(V_ViewPos.z / V_ViewPos.w));
    gl_FragColor = TextureCube() * (diffuse + specular) * alpha;
    // gl_FragColor = vec4(alpha);
}