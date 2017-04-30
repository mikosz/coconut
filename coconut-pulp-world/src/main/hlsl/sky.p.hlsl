#include "sky.hlsl"

TextureCube skydome;
SamplerState skydomeSampler;

float4 main(VertexOut pin) : SV_TARGET {
	return skydome.Sample(skydomeSampler, pin.posL);
}
