#include "grass-common.hlsl"

static const float WIND_SCALE = 0.2f;

cbuffer PatchData {
	float3 actor_grassPatchPosition;
};

cbuffer TerrainData {
	float terrain_width;
	float terrain_depth;
};

Texture2D material_noiseMap;

Texture2D terrain_heightmap;
SamplerState terrain_heightmapSampler;

Texture2D windmap_texture;
SamplerState windmap_sampler;

GIn main(uint bladeId : SV_VertexID)
{
	static const float OFFSET = 0.05f;
	static const uint BLADES_PER_ROW = 400;
	static const float HALF_OFFSET = OFFSET * 0.5f;
	static const uint TEXTURE_WIDTH = 800;
	static const uint TEXTURE_HEIGHT = 600;

	GIn vout;
	
	const uint columnId = bladeId % BLADES_PER_ROW;
	const uint rowId = bladeId / BLADES_PER_ROW;
	
	const float4 noise = material_noiseMap[uint2(rowId % TEXTURE_WIDTH, columnId % TEXTURE_HEIGHT)];
			
	vout.posW = actor_grassPatchPosition;
	vout.posW.x += (columnId) * OFFSET;
	vout.posW.z += (rowId) * OFFSET;

	const float halfWidth = terrain_width * 0.5f;
	const float halfDepth = terrain_depth * 0.5f;
	const float2 terrainTexcoord = float2(
		(vout.posW.x + halfWidth) / terrain_width,
		(vout.posW.z + halfDepth) / terrain_depth
		);
	vout.posW.y += terrain_heightmap.SampleLevel(terrain_heightmapSampler, terrainTexcoord, 0).r;

	//vout.posW.x += noise.x * HALF_OFFSET;
	//vout.posW.z += noise.z * HALF_OFFSET;
	vout.posW.x += noise.x * OFFSET;
	vout.posW.z += noise.z * OFFSET;

	vout.windDir = WIND_SCALE * windmap_texture.SampleLevel(windmap_sampler, terrainTexcoord, 0).rg;
	
	vout.noiseVal = noise.y;

	return vout;
}
