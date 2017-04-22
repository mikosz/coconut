cbuffer ViewTransformations : register(b0) {
	matrix viewMatrix;
}

cbuffer ProjectionTransformations : register(b1) {
	matrix projectionMatrix;
}

Buffer<float3> grassPatchPositions;

Texture2D noiseMap : register(t0);
SamplerState noiseMapSamplerState : register(s0);

float4 main(uint vertexId : SV_VertexID) : SV_POSITION
{
	float3 patchPosW = grassPatchPositions[vertexId]; // TODO
	float4 posW = float4(patchPosW, 1.0f);

	float4 noise = noiseMap.SampleLevel(noiseMapSamplerState, patchPosW.xz * 0.01f, 0);
	posW.x += noise.x;
	posW.y *= 1.0f + noise.y;
	posW.z += noise.z;

	return mul(mul(posW, viewMatrix), projectionMatrix);
}
