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

VOut main(VIn vin)
{
	vin.posL.w = 1.0f;

	VOut vout;

	float4 posW = vin.posL + float4(vin.patchPosW, 1.0f);

	float4 noise_fac = frac((posW.z * 0.5 + 0.5) * float4(1923, 338, 879,1631)) * 0.1f;
	float2 noise_fac2 = frac((posW.z * 0.5 + 0.5) * float2(493, 579)) * 10.0f;

	posW.x += noise_fac.x;
	posW.z += noise_fac.z;

	vout.posH = mul(mul(posW, viewMatrix), projectionMatrix);
	vout.tex = vin.tex + noise_fac2;
	vout.posW = posW;
	vout.normalW = vin.normalL; // mul(vin.normalL, (float3x3)(worldInvTranspose));

	return vout;
}
