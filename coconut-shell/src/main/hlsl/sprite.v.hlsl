cbuffer WorldTransformations : register(b0) {
	matrix worldMatrix;
	matrix worldInvTranspose;
}

cbuffer ViewTransformations : register(b1) {
	matrix viewMatrix; // TODO: one world-view-proj matrix would suffice in b0 (and would be quicker)
}

cbuffer ProjectionTransformations : register(b2) {
	matrix projectionMatrix;
}

struct VIn {
	float4 posL : POSITION;
	float2 tex : TEXCOORD;
	float3 normalL : NORMAL;
	float4 patchPosW: PATCH_POSITION;
};

struct VOut {
	float4 posH : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
};

VOut main(VIn vin)
{
	vin.posL.w = 1.0f;

	VOut vout;

	vout.posH = mul(mul(mul(vin.posL, worldMatrix), viewMatrix), projectionMatrix);
	vout.tex = vin.tex;
	vout.posW = mul(vin.posL, worldMatrix).xyz;
	vout.normalW = mul(vin.normalL, (float3x3)(worldInvTranspose));

	return vout;
}
