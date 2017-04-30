#include "sky.hlsl"

//TextureCube skydome;
//SamplerState skydomeSampler;

float4 main(VertexOut pin) : SV_TARGET {
	return float4(0.0f, 0.0f, 1.0f, 1.0f);
//	return skydome.Sample(skydomeSampler, pin.posL);
}
