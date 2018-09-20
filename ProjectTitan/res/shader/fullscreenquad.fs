#ifdef GL_ES
precision mediump float;
#endif

#define OFFSET 		400.0
#define COUNT 		10

uniform sampler2D U_Texture;
uniform sampler2D U_BackgroundTexture;
uniform sampler2D U_AlphaMap;

uniform vec4 U_Option;
uniform vec4 U_Critical; // left -> top -> right -> bottom

varying vec2 V_Texcoord;
varying vec2 V_Position;

vec4 CalcHBlur()
{
	float offset = 1.0 / OFFSET;
	float weight[COUNT];
	int index = 0;
	float n = 1.0;
	float t = n / float(weight.length - 1) * 2.0;
	float d = t / float(weight.length - 1);
	for(int i = int(weight.length) - 1; i >= 1; --i)
	{
		weight[index++] = float(i)*d;
	}

	vec4 color = texture2D(U_Texture, V_Texcoord) * weight[0];

	for(int i = 0; i < weight.length; ++i)
	{
		color += texture2D(U_Texture, vec2(V_Texcoord.x + (offset*float(i)*pow(-1.0, float(i))),V_Texcoord.y)) * weight[i];
	}
	return color;
}

vec4 CalcVBlur()
{
	float offset = 1.0 / OFFSET;
	float weight[COUNT];
	int index = 0;
	float n = 1.0;
	float t = n / float(weight.length - 1) * 2.0;
	float d = t / float(weight.length - 1);
	for(int i = int(weight.length) - 1; i >= 1; --i)
	{
		weight[index++] = float(i)*d;
	}

	vec4 color = texture2D(U_Texture, V_Texcoord) * weight[0];

	for(int i = 0; i < weight.length; ++i)
	{
		color += texture2D(U_Texture, vec2(V_Texcoord.x,V_Texcoord.y + (offset*float(i)*pow(-1.0, float(i))))) * weight[i];
	}
	return color;
}

vec4 CompressImg(float coefficient)
{	
	float offsetX = mod(V_Texcoord.x, coefficient);
	float offsetY = mod(V_Texcoord.y, coefficient);
	vec4 color = texture2D(U_Texture, vec2(V_Texcoord.x - offsetX, V_Texcoord.y - offsetY));
	return color;
}

void main()
{

	vec4 color = vec4(0.0);
	if(U_Option.x > 0.0)
	{
		color = CalcHBlur();
	}
	else if(U_Option.y > 0.0)
	{
		color = CalcVBlur();
	}
	else
	{
		color = texture2D(U_Texture, V_Texcoord);
	}
	
	if(U_Option.z > 0.0)
	{
		float alpha = texture2D(U_AlphaMap, V_Texcoord).r;

		// float focus = U_Option.z;
		// alpha = abs(focus - alpha);

		color = color * alpha;

		vec4 bg = texture2D(U_BackgroundTexture, V_Texcoord);
		alpha = 1.0 - alpha;
		color = color + bg * alpha;
		gl_FragColor = vec4(color);
		return;
	}
	else
	{
		gl_FragColor = color;
		return;
	}
}