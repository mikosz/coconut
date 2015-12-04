cbuffer Material : register(b0) {
	float4 diffuseColour;
}

Texture2D diffuseMap;

SamplerState ss {
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
};

struct PIn {
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

float4 main(PIn pin) : SV_TARGET
{
	return saturate(/*diffuseColour */ float4(1.0f, 1.0f, 1.0f, 1.0f) * diffuseMap.Sample(ss, pin.tex));
}
