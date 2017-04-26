cbuffer PatchData : register(b0) {
	float3 grassPatchPosition;
};

Texture2D noiseMap : register(t0);

struct VOut {
	float3 posW : POSITION;
	float height : BLADE_HEIGHT;
};

VOut main(uint bladeId : SV_VertexID)
{
	static const float OFFSET = 0.05f;
	static const uint BLADES_PER_ROW = 400;
	static const float HALF_OFFSET = OFFSET * 0.5f;
	static const uint TEXTURE_WIDTH = 800;
	static const uint TEXTURE_HEIGHT = 600;

	VOut vout;
	
	const uint columnId = bladeId % BLADES_PER_ROW;
	const uint rowId = bladeId / BLADES_PER_ROW;
	
	const float4 noise = noiseMap[uint2(rowId % TEXTURE_WIDTH, columnId % TEXTURE_HEIGHT)];
	
	vout.posW = grassPatchPosition;
	vout.posW.x += (columnId) * OFFSET;
	vout.posW.z += (rowId) * OFFSET;

	vout.posW.x += noise.x * HALF_OFFSET;
	vout.posW.z += noise.z * HALF_OFFSET;

	vout.height = 0.15f + noise.y;

	return vout;
}
