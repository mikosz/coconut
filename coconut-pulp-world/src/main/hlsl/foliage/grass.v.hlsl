struct SceneData {
	matrix view;
	matrix projection;
};

cbuffer SceneBuffer {
	SceneData scene;
};

cbuffer ObjectData {
	float3 grassPatchPosition;
}

struct VIn {
	float4 posL : POSITION;
	float2 tex : TEXCOORD;
	float3 normalL : NORMAL;
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
	float3 patchPosW = grassPatchPosition;

	vin.posL.w = 1.0f;

	VOut vout;

	float4 posW = vin.posL + float4(patchPosW, 1.0f);

	float4 noise = noiseMap.SampleLevel(noiseMapSamplerState, patchPosW.xz * 0.01f, 0);
	posW.x += noise.x;
	posW.y *= 1.0f + noise.y;
	posW.z += noise.z;

	vout.posH = mul(mul(posW, scene.view), scene.projection);
	vout.tex = vin.tex;
	vout.posW = posW.xyz;
	vout.normalW = vin.normalL;

	return vout;
}
