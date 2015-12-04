cbuffer WorldTransformations : register(b0) {
	matrix worldMatrix;
}

cbuffer ViewTransformations : register(b1) {
	matrix viewMatrix;
}

cbuffer ProjectionTransformations : register(b2) {
	matrix projectionMatrix;
}

struct VIn {
	float4 pos : POSITION;
	float2 tex : TEXCOORD;
};

struct VOut {
	float4 pos : SV_POSITION;
	float2 tex : TEXCOORD;
};

VOut main(VIn vin)
{
	vin.pos.w = 1.0f;

	VOut vout;

	vout.pos = mul(mul(mul(vin.pos, worldMatrix), viewMatrix), projectionMatrix);
	vout.tex = vin.tex;

	return vout;
}
