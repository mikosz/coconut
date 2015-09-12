Texture2D tex;
SamplerState samplerState;

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 textureCoord : TEXCOORD0;
};

float4 main(PixelInputType input) : SV_TARGET
{
	return tex.Sample(samplerState, input.textureCoord);
}
