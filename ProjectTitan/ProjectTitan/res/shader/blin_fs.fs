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
	L = normalize(L);
	vec3 N = V_Normal;
	N = normalize(N);
	vec4 diffuse = V_DiffuseLight*V_DiffuseMaterial*max(0.0, dot(L, N));
	
	vec4 specular = vec4(0.0);
	vec4 worldPos = V_WorldPos;
	if(diffuse.x > 0.0)
	{
		vec3 viewDir = normalize(V_CameraPos - worldPos).xyz;
		vec3 halfL = normalize(viewDir + L);
		specular = V_SpecularMaterial*V_SpecularLight*pow(max(0.0, dot(N, halfL)), 8.0);
	}
	gl_FragColor = V_AmbientMaterial*V_AmbientLight + diffuse + specular;
}