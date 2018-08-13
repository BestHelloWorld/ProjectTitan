#ifdef GL_ES
precision mediump float;
#endif

uniform vec4 V_AmbientLight;
uniform vec4 V_DiffuseLight;
uniform vec4 V_SpecularLight;

uniform vec4 V_CameraPos;
uniform vec4 V_LightPos;

uniform vec4 V_LightDirection;
uniform vec4 V_LightOption;

varying vec3 V_Normal;
varying vec4 V_WorldPos;

void main()
{
	vec3 L = V_LightPos.xyz - V_WorldPos.xyz;
	vec3 N = V_Normal;
	N = normalize(N);

	float distance = 0.0;
	float attenuation = 1.0;
	float constantFactor = 0.5;
	float linearFactor = 0.3;
	float expFactor = 0.1;
	distance = length(L);
	attenuation = 1.0f;//2.0 / (constantFactor + linearFactor * distance + expFactor*expFactor*distance);
	L = normalize(L);
	
	float diffuse = max(0.0, dot(L, N))*attenuation;
	float specular = 0.0;	
	if(diffuse > 0.0)
	{
		vec3 LightDir = normalize(V_LightDirection.xyz);
		float cutoff = max(0.0, dot(LightDir, -normalize(V_LightPos.xyz - V_WorldPos.xyz)));
		float theta = cos(V_LightDirection.w*3.1415926/180.0);
				
		// if(cutoff > theta)
		{
			vec3 reflectLight = reflect(-L, N);
			vec3 viewDir = -V_WorldPos.xyz;
			specular = pow(max(0.0, dot(reflectLight, viewDir)), 4.0);
		}
		// else
		{
			// diffuse = 0.0;	
		}
	}
	
	vec4 diffuseColor =  vec4(1.0)*diffuse;
	vec4 specularColor = vec4(1.0)*specular;
	gl_FragColor =  specularColor +  0.05;
}