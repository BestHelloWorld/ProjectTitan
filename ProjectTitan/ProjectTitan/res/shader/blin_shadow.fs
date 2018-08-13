#ifdef GL_ES
precision mediump float;
#endif

uniform vec4 U_AmbientMaterial;
uniform vec4 U_DiffuseMaterial;
uniform vec4 U_SpecularMaterial;

uniform vec4 U_CameraPos;
uniform vec4 U_LightPos;

uniform sampler2D U_ShadowMap;
uniform sampler2D U_Texture;

varying vec2 V_Texcoord;
varying vec4 V_Normal;
varying vec4 V_WorldPos;
varying vec4 V_LightFragSpacePos;

float CalcShadow()
{
	vec3 fragPos = V_LightFragSpacePos.xyz;
	if(fragPos.x <= -1.0 || fragPos.y <= -1.0 || fragPos.x >= 1.0 || fragPos.y >= 1.0)
		return 0.0;

	fragPos = V_LightFragSpacePos.xyz / V_LightFragSpacePos.w;
	fragPos = (fragPos + 1.0) * 0.5;
		
	float depth = texture2D(U_ShadowMap, fragPos.xy).r;
	float currentDepth = fragPos.z;
	if(depth + 0.01 < currentDepth)
	{
		return 1.0;
	}
	else
	{
		return 0.0;
	}
}

void main()
{
	float depth = CalcShadow();
	
	float attenuation = 1.0;
	if(U_LightPos.w > 0.0)
	{
		float distance = length(V_WorldPos.xyz - U_LightPos.xyz);
		attenuation = 1.0 / (0.5 + 0.3 * distance + 0.1 * 0.1 * distance);
	}

	vec3 L = U_LightPos.xyz;
	L = normalize(L);
	vec3 N = V_Normal.xyz;
	N = normalize(N);

	vec4 diffuse = U_DiffuseMaterial * max(0.0, dot(L, N)) * attenuation;
	
	vec4 specular = vec4(0.0);
	vec4 worldPos = V_WorldPos;
	if(diffuse.x > 0.0)
	{
		vec3 viewDir = normalize(U_CameraPos - worldPos).xyz;
		vec3 halfL = normalize(viewDir + L);
		specular = U_SpecularMaterial * pow(max(0.0, dot(N, halfL)), 8.0);
	}
	if(texture2D(U_Texture, V_Texcoord) == vec4(0.0, 0.0, 0.0, 1.0))
	{
		gl_FragColor = (U_AmbientMaterial + diffuse + specular);
		return;
	}

	if(depth > 0.0)
	{
		gl_FragColor = texture2D(U_Texture, V_Texcoord)*(U_AmbientMaterial + diffuse + specular) * 0.5;
		return;
	}
	gl_FragColor = texture2D(U_Texture, V_Texcoord)*(U_AmbientMaterial + diffuse + specular);

}