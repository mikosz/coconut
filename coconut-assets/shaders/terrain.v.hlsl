#include "terrain.hlsl"

Texture2D terrain_heightmap;
SamplerState terrain_heightmapSampler;

VertexInOut main(VertexInOut v)
{
	v.posW.y = terrain_heightmap.SampleLevel(terrain_heightmapSampler, v.heightmapTexcoord, 0).r;
	return v;
}
