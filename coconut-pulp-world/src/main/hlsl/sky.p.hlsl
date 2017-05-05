#include "sky.hlsl"

TextureCube material_skydome;
SamplerState material_skydomeSampler;

float4 main(VertexOut pin) : SV_TARGET {
	return material_skydome.Sample(material_skydomeSampler, pin.posL);
}
