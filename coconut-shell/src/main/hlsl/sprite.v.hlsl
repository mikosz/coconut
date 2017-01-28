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
	uint instanceID : SV_InstanceID;
};

struct VOut {
	float4 posH : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
};

VOut main(VIn vin)
{
	matrix actWorld = worldMatrix;
	matrix actWorldIT = worldInvTranspose;

	if (vin.instanceID == 1) {
		matrix trans = { 1, 0, 0, 0,
						 0, 1, 0, 0,
						 0, 0, 1, 0,
						 10, 10, 10, 1 };
		actWorld = worldMatrix * trans;

		matrix itTrans = { 1, 0, 0, -10,
						   0, 1, 0, -10,
						   0, 0, 1, -10,
						   0, 0, 0, 1 };
		actWorldIT = actWorldIT * itTrans;
	}

	vin.posL.w = 1.0f;

	VOut vout;

	vout.posH = mul(mul(mul(vin.posL, actWorld), viewMatrix), projectionMatrix);
	vout.tex = vin.tex;
	vout.posW = mul(vin.posL, actWorld).xyz;
	vout.normalW = mul(vin.normalL, (float3x3)(actWorldIT));

	return vout;
}
