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

uniform vec4 V_AmbientMaterial;
uniform vec4 V_AmbientLight;

uniform vec4 V_DiffuseMaterial;
uniform vec4 V_DiffuseLight;

uniform vec4 V_SpecularMaterial;
uniform vec4 V_SpecularLight;

uniform vec4 V_CameraPos;
uniform vec4 V_LightPos;

varying vec4 V_Color;

void main()
{
	vec3 L = V_LightPos.xyz;
	L = normalize(L);
	vec3 N = (NormalMatrix*normal).xyz;
	N = normalize(N);
	vec4 diffuse = V_DiffuseLight*V_DiffuseMaterial*max(0.0, dot(L, N));
	
	vec4 specular = vec4(0.0);
	vec4 worldPos = ModelMatrix*position;
	if(diffuse.x > 0.0)
	{
		vec3 viewDir = normalize(V_CameraPos - worldPos).xyz;
		vec3 halfL = normalize(viewDir + L);
		specular = V_SpecularMaterial*V_SpecularLight*pow(max(0.0, dot(N, halfL)), 8.0);
	}
	
	V_Color = V_AmbientMaterial*V_AmbientLight + diffuse + specular;
	gl_Position = ProjectMatrix*ViewMatrix*worldPos;
}