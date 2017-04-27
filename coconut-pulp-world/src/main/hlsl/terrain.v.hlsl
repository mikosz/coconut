#include "terrain.hlsl"

Texture2D heightMap;
SamplerState heightMapSampler;

VertexInOut main(VertexInOut v)
{
	v.posW.y = heightMap.SampleLevel(heightMapSampler, v.heightmapTexcoord, 0).r;
	return v;
}
