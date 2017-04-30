#include "terrain.hlsl"

Texture2D heightmap;
SamplerState heightmapSampler;

VertexInOut main(VertexInOut v)
{
	v.posW.y = heightmap.SampleLevel(heightmapSampler, v.heightmapTexcoord, 0).r;
	return v;
}
