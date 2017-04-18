cbuffer ViewTransformations : register(b0) {
	matrix viewMatrix; // TODO: one world-view-proj matrix would suffice in b0 (and would be quicker)
}

cbuffer ProjectionTransformations : register(b1) {
	matrix projectionMatrix;
}

struct VIn {
	float4 posL : POSITION;
	float2 tex : TEXCOORD;
	float3 normalL : NORMAL;
	float3 patchPosW: GRASS_PATCH_POSITION;
};

struct VOut {
	float4 posH : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
};

Texture2D noiseMap : register(t0);

VOut main(VIn vin)
{
	vin.posL.w = 1.0f;

	VOut vout;

	float4 posW = vin.posL + float4(vin.patchPosW, 1.0f);
	posW += noiseMap[uint2(0, 0)];

	vout.posH = mul(mul(posW, viewMatrix), projectionMatrix);
	vout.tex = vin.tex;
	vout.posW = posW;
	vout.normalW = vin.normalL;

	return vout;
}
