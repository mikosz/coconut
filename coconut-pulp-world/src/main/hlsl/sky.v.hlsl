#include "sky.hlsl"

struct SceneData {
	matrix view;
	matrix projection;
};

cbuffer SceneBuffer {
	SceneData scene;
};

struct VertexIn {
	float3 posL : POSITION;
};

VertexOut main(VertexIn vin) {
	matrix viewNoTranslation = scene.view; // TODO
	viewNoTranslation[3] = float4(0.0f, 0.0f, 0.0f, 1.0f);

	VertexOut vout;
	// TODO: deLuna does .xyww, but it doesn't work?
	vout.posH = mul(mul(float4(vin.posL, 1.0f), viewNoTranslation), scene.projection);// .xyww;
	vout.posL = vin.posL;

	return vout;
}
