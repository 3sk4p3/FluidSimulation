#version 450 core

layout (location = 0) out vec4 o_Color;

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

uniform sampler2D u_Textures[3];

void main()
{
	int index=int(v_TexIndex);
	if(index==2)
	{
	o_Color=vec4(1.0f);	
	}
	else
	{
	o_Color =texture(u_Textures[2],v_TexCoord);
	}

}