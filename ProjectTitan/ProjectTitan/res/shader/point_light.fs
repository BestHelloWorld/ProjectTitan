#ifdef GL_ES
precision mediump float;
#endif

uniform vec4 V_AmbientMaterial;
uniform vec4 V_AmbientLight;
uniform vec4 V_DiffuseMaterial;
uniform vec4 V_DiffuseLight;
uniform vec4 V_SpecularMaterial;
uniform vec4 V_SpecularLight;

uniform vec4 V_CameraPos;
uniform vec4 V_LightPos;

varying vec3 V_Normal;
varying vec4 V_WorldPos;

void main()
{
	vec3 L = V_LightPos.xyz;
	vec3 N = V_Normal;
	N = normalize(N);
	
	float distance = 0.0;
	float attenuation = 1.0;
	float constantFactor = 0.5;
	float linearFactor = 0.3;
	float expFactor = 0.1;
	distance = length(L - V_WorldPos.xyz);
	attenuation = 1.0 / (constantFactor + linearFactor * distance + expFactor*expFactor*distance);
	L = normalize(L);
	
	vec4 diffuse = V_DiffuseMaterial*V_DiffuseLight*max(0.0, dot(L, N)*attenuation);
	
	gl_FragColor = diffuse;
}