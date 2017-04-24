cbuffer ViewTransformations : register(b0) {
	matrix viewMatrix;
}

cbuffer ProjectionTransformations : register(b1) {
	matrix projectionMatrix;
}

static float3 verts[4] = {
	float3(-0.03f, 0.0f, 0.0f),
	float3(-0.03f, 0.15f, 0.0f),
	float3(0.03f, 0.15f, 0.0f),
	float3(0.03f, 0.0f, 0.0f),
	};

Buffer<float3> grassPatchPositions;

Texture2D noiseMap : register(t0);
SamplerState noiseMapSamplerState : register(s0);

struct VOut {
	float4 posH : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
};

VOut main(uint vertexId : SV_VertexID)
{
	VOut vout;
	
	float3 patchPosW = grassPatchPositions[vertexId / 4];
	float4 posW = float4(verts[vertexId % 4] + patchPosW, 1.0f);

	float4 noise = noiseMap.SampleLevel(noiseMapSamplerState, patchPosW.xz * 0.01f, 0);
	//posW.x += noise.x;
	//posW.y *= 1.0f + noise.y;
	//posW.z += noise.z;

	vout.posH = mul(mul(posW, viewMatrix), projectionMatrix);
	vout.posW = posW.xyz;
	vout.normalW = float3(0.0f, 0.0f, -1.0f);

	return vout;
}
