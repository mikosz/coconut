#include "sky.hlsl"

cbuffer ViewTransformations {
	matrix viewMatrix;
}

cbuffer ProjectionTransformations {
	matrix projectionMatrix;
}

struct VertexIn {
	float3 posL : POSITION;
};

VertexOut main(VertexIn vin) {
	matrix viewNoTranslation = viewMatrix; // TODO
	viewNoTranslation[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);

	VertexOut vout;
	// TODO: deLuna does .xyww, but it doesn't work?
	vout.posH = mul(mul(float4(vin.posL, 1.0f), viewNoTranslation), projectionMatrix);// .xyww;
	vout.posL = vin.posL;

	return vout;
}
