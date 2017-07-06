#include "sky.hlsl"

cbuffer scene {
	matrix view;
	matrix projection;
};

struct VertexIn {
	float3 posL : POSITION;
};

VertexOut main(VertexIn vin) {
	matrix viewNoTranslation = view; // TODO
	viewNoTranslation[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);

	VertexOut vout;
	// TODO: deLuna does .xyww, but it doesn't work?
	vout.posH = mul(mul(float4(vin.posL, 1.0f), viewNoTranslation), projection);// .xyww;
	vout.posL = vin.posL;

	return vout;
}
