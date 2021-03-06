#include "terrain.hlsl"

cbuffer scene {
	matrix view;
	matrix projection;
};

Texture2D terrain_heightmap;
SamplerState terrain_heightmapSampler;

static float tiledTextureScale = 50.0f;

[domain("quad")]
DomainOut main(
	PatchTesselation pt,
	float2 uv : SV_DomainLocation,
	const OutputPatch<HullOut, 4> quad
	)
{
	DomainOut dout;

	dout.heightmapTexcoord = lerp(
		lerp(quad[0].heightmapTexcoord, quad[1].heightmapTexcoord, uv.x),
		lerp(quad[2].heightmapTexcoord, quad[3].heightmapTexcoord, uv.x),
		uv.y
		);

	dout.tiledTexcoord = dout.heightmapTexcoord * tiledTextureScale;

	dout.posW = lerp(
		lerp(quad[0].posW, quad[1].posW, uv.x),
		lerp(quad[2].posW, quad[3].posW, uv.x),
		uv.y
		);

	dout.posW.y = terrain_heightmap.SampleLevel(terrain_heightmapSampler, dout.heightmapTexcoord, 0).r;

	dout.posH = mul(mul(float4(dout.posW, 1.0f), view), projection);

	return dout;
}
