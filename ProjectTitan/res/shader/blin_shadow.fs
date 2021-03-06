#ifdef GL_ES
precision mediump float;
#endif

#define OFFSET 1.0 / 300.0
#define COUNT 4

uniform vec4 U_AmbientMaterial;
uniform vec4 U_DiffuseMaterial;
uniform vec4 U_SpecularMaterial;

// LIGHT
uniform vec4 U_CameraPos;
uniform vec4 U_LightPos;

// FOG
uniform vec4 U_FogLimit;
uniform vec4 U_FogColor;

uniform sampler2D U_ShadowMap;
uniform sampler2D U_Texture;

varying vec2 V_Texcoord;
varying vec4 V_Normal;
varying vec4 V_WorldPos;
varying vec4 V_ProjectionPos;
varying vec4 V_LightFragSpacePos;

float CalcShadow()
{
	vec3 fragPos = V_LightFragSpacePos.xyz;
	if(fragPos.x <= -1.0 || fragPos.y <= -1.0 || fragPos.x >= 1.0 || fragPos.y >= 1.0)
		return 0.0;

	fragPos = V_LightFragSpacePos.xyz / V_LightFragSpacePos.w;
	fragPos = (fragPos + 1.0) * 0.5;
	
	float currentDepth = fragPos.z;
	float depth = 0.0;

	if(!(texture2D(U_ShadowMap, fragPos.xy).r + 0.0001 <= currentDepth))
		return 0.0;

//	vec2 offset = vec2(OFFSET);
	for(int i = -COUNT / 2; i < COUNT / 2; ++i)
	{
		for(int j = -COUNT / 2; j < COUNT / 2; ++j)
		{
			if(texture2D(U_ShadowMap, vec2(fragPos.x + OFFSET * i, fragPos.y + OFFSET * j)).r + 0.0001 <= currentDepth)
				depth += 1.0;
		}
	}

	return depth / 16.0;
//	if(depth + 0.0001 <= currentDepth)
//		return 1.0;
//	else if(depth + 0.0005 <= currentDepth)
//		return 0.5;
//	else
//		return 0.0;
}

float CalcFog(float distance, float near, float far)
{
    float alpha = (distance) / (far - near);
    alpha = clamp(alpha, 0.0, 1.0);
    return alpha;
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

	// CALC DIFFUSE
	vec4 diffuse = U_DiffuseMaterial * max(0.0, dot(L, N)) * attenuation;
	
	// CALC BLIN SPECULAR
	vec4 specular = vec4(0.0);
	vec4 worldPos = V_WorldPos;
	//if(diffuse.x > 0.0)
	{
		vec3 viewDir = normalize(U_CameraPos - worldPos).xyz;
		vec3 halfL = normalize(viewDir + L);
		specular = U_SpecularMaterial * pow(max(0.0, dot(N, halfL)), 16.0);
	}

	// TEXTURE
	vec4 color;
	if(texture2D(U_Texture, V_Texcoord) == vec4(0.0, 0.0, 0.0, 1.0))
	{
		gl_FragColor = (U_AmbientMaterial + diffuse + specular);
		return;
	}

	// CALC SHADOW
	if(depth > 0.0)
		color = texture2D(U_Texture, V_Texcoord)*(U_AmbientMaterial + diffuse + specular) * min((1.0 - depth) + 0.5, 1.0);
	else
		color = texture2D(U_Texture, V_Texcoord)*(U_AmbientMaterial + diffuse + specular);


	// CALC FOG
	if(U_FogLimit.y != 0.0)
	{
		float delta = CalcFog(abs(V_ProjectionPos.z), U_FogLimit.x, U_FogLimit.y);
		color = (color * (1.0 - delta))/* + (delta * U_FogColor)*/;
	}

	gl_FragColor = color;
}