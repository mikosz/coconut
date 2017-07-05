cbuffer PatchData {
	float3 actor_grassPatchPosition;
};

Texture2D material_noiseMap;

struct VOut {
	float3 posW : POSITION;
	float noiseVal : NOISE;
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
	
	const float4 noise = material_noiseMap[uint2(rowId % TEXTURE_WIDTH, columnId % TEXTURE_HEIGHT)];
	
	vout.posW = actor_grassPatchPosition;
	vout.posW.x += (columnId) * OFFSET;
	vout.posW.z += (rowId) * OFFSET;

	//vout.posW.x += noise.x * HALF_OFFSET;
	//vout.posW.z += noise.z * HALF_OFFSET;
	vout.posW.x += noise.x * OFFSET;
	vout.posW.z += noise.z * OFFSET;

	vout.noiseVal = noise.y;

	return vout;
}