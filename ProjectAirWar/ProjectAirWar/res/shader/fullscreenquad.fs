#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D U_Texture;
uniform vec4 U_Option;
uniform vec4 U_Critical; // left -> top -> right -> bottom

varying vec2 V_Texcoord;
varying vec2 V_Position;

vec4 CalcHBlur()
{
	float offset = 1.0 / 200.0;
	float weight[20];
	int index = 0;
	float n = 1.0;
	float t = n / float(weight.length - 1) * 2.0;
	float d = t / float(weight.length - 1);
	for(int i = weight.length - 1; i >= 1; --i)
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
	float offset = 1.0 / 200.0;
	float weight[20];
	int index = 0;
	float n = 1.0;
	float t = n / float(weight.length - 1) * 2.0;
	float d = t / float(weight.length - 1);
	for(int i = weight.length - 1; i >= 1; --i)
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
	if (V_Position.x > U_Critical.x && V_Position.y < U_Critical.y &&
		V_Position.x < U_Critical.z && V_Position.y > U_Critical.w)
	{
		gl_FragColor = texture2D(U_Texture, V_Texcoord);
		return;
	}
	// if (V_Position.x > -1.0 && V_Position.y < 1.0 &&
	// 	V_Position.x < 1.0 && V_Position.y > -1.0)
	// {
	// 	gl_FragColor = texture2D(U_Texture, V_Texcoord);
	// 	return;
	// }


	if(U_Option.x > 0.0)
	{
		gl_FragColor = CompressImg(U_Option.x);
		return;
	}
	else if(U_Option.y > 0.0)
	{
		gl_FragColor = CalcHBlur();
		return;
	}
	else if(U_Option.z > 0.0)
	{
		gl_FragColor = CalcVBlur();
		return;
	}
	else
	{
		gl_FragColor = texture2D(U_Texture, V_Texcoord);
	}
}