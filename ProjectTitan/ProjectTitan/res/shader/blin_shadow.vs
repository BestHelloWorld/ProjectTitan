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
varying vec4 V_LightFragSpacePos;

void main()
{
	V_Texcoord = texcoord;
	
	if(U_WaterOption.x != 0.0)
	{
		position.y = sin(position.x + U_WaterOption.y) * U_WaterOption.x + U_WaterOption.z;
		V_Normal.x = (sin(position.x - 0.5 + U_WaterOption.y) * U_WaterOption.x) - 
						(sin(position.x + 0.5 + U_WaterOption.y) * U_WaterOption.x);
		V_Normal.y = 2.0;
		V_Normal.z = 0.0;
		V_Normal = normalize(V_Normal);
	}
	else
	{
		V_Normal = NormalMatrix*normal;
	}
		
	V_WorldPos = ModelMatrix*position;
	V_LightFragSpacePos = LightProjectMatrix*LightViewMatrix*V_WorldPos;
	gl_Position = ProjectMatrix*ViewMatrix*V_WorldPos;
}