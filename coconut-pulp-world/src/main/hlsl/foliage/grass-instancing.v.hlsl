cbuffer ViewTransformations : register(b0) {
	matrix viewMatrix;
}

cbuffer ProjectionTransformations : register(b1) {
	matrix projectionMatrix;
}

cbuffer ObjectData : register(b2) {
	float3 grassPatchPosition;
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
SamplerState noiseMapSamplerState : register(s0);

VOut main(VIn vin)
{
	float3 patchPosW = vin.patchPosW;

	vin.posL.w = 1.0f;

	VOut vout;

	float4 posW = vin.posL + float4(patchPosW, 1.0f);

	float4 noise = noiseMap.SampleLevel(noiseMapSamplerState, patchPosW.xz * 0.01f, 0);
	posW.x += noise.x;
	posW.y *= 1.0f + noise.y;
	posW.z += noise.z;

	vout.posH = mul(mul(posW, viewMatrix), projectionMatrix);
	vout.tex = vin.tex;
	vout.posW = posW.xyz;
	vout.normalW = vin.normalL;

	return vout;
}
