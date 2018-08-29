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

uniform mat4 LightViewMatrix;
uniform mat4 LightProjectMatrix;

uniform vec4 U_WaterOption;

varying vec2 V_Texcoord;
varying vec4 V_Normal;
varying vec4 V_WorldPos;
varying vec4 V_ProjectionPos;
varying vec4 V_LightFragSpacePos;

void main()
{
	V_Texcoord = texcoord.xy;
	
	vec4 newPos = position;
	vec4 newNormal = normal;
	if(U_WaterOption.x != 0.0)
	{
		newPos.y = sin(newPos.x + U_WaterOption.y) * U_WaterOption.x + U_WaterOption.z;
		V_Normal.x = (sin(newPos.x - 0.5 + U_WaterOption.y) * U_WaterOption.x) - 
						(sin(newPos.x + 0.5 + U_WaterOption.y) * U_WaterOption.x);
		V_Normal.y = 2.0;
		V_Normal.z = 0.0;
		V_Normal = normalize(V_Normal);
	}
	else
	{
		V_Normal = NormalMatrix*normal;
	}
		
	V_WorldPos = ModelMatrix*newPos;
	V_LightFragSpacePos = LightProjectMatrix*LightViewMatrix*V_WorldPos;
	V_ProjectionPos = ProjectMatrix*ViewMatrix*V_WorldPos;
	gl_Position = V_ProjectionPos;
}