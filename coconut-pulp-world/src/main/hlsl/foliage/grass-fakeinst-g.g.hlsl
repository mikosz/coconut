struct SceneData {
	matrix view;
	matrix projection;
};

cbuffer SceneBuffer {
	SceneData scene;
	float globalTime;
};

cbuffer TerrainData {
	float terrain_width;
	float terrain_depth;
};

struct GIn {
	float3 posW : POSITION;
	float noiseVal : NOISE;
};

struct GOut {
	float4 posH : SV_POSITION;
	float3 posW : POSITION;
	float noiseVal : NOISE;
	float3 normalW : NORMAL;
	float2 tex : TEXCOORD;
};

static float tiledTextureScale = 50.0f;

Texture2D terrain_heightmap;
SamplerState terrain_heightmapSampler;

Texture2D terrain_tiledTexture;
SamplerState terrain_tiledTextureSampler;

static const float3 verts[] = {
	float3(-0.005f, 0.0f, 0.0f),
	float3(0.005f, 0.0f, 0.0f),
	float3(-0.005f, 0.40f, 0.0f),
	float3(0.005f, 0.40f, 0.0f),
};

static const float2 texcoords[] = {
	float2(0.0f, 1.0f),
	float2(1.0f, 1.0f),
	float2(0.0f, 0.0f),
	float2(1.0f, 0.0f),
};

//static const float y_step = 0.15f * 0.25f;
//static const float3 verts[] = {
//	float3(-0.003f, 0.0f, 0.0f),
//	float3(0.003f, 0.0f, 0.0f),
//	float3(-0.003f, y_step, 0.0f),
//	float3(0.003f, y_step, 0.0f),
//	float3(-0.003f, 2.0f * y_step, 0.0f),
//	float3(0.003f, 2.0f * y_step, 0.0f),
//	float3(-0.003f, 3.0f * y_step, 0.0f),
//	float3(0.003f, 3.0f * y_step, 0.0f),
//	float3(-0.003f, 4.0f * y_step, 0.0f),
//	float3(0.003f, 4.0f * y_step, 0.0f),
//};

static const uint SEGMENTS = 1;

void emit(
	GIn gin,
	inout GOut gout,
	inout TriangleStream<GOut> triStream,
	uint index,
	float terrainHeight,
	float3 windFactor
	)
{
	gout.tex = texcoords[index];
	gout.posW = gin.posW + verts[index];
	gout.posW.y *= 1.0f + (gin.noiseVal * 0.05f);
	gout.posW.y += terrainHeight;
	gout.posW += windFactor;
	gout.posH = mul(mul(float4(gout.posW, 1.0f), scene.view), scene.projection);
	triStream.Append(gout);
}

[maxvertexcount(6 * SEGMENTS)]
void main(point GIn gin[1], inout TriangleStream<GOut> triStream) {
	const float halfWidth = terrain_width * 0.5f;
	const float halfDepth = terrain_depth * 0.5f;
	const float2 heightmapTexcoord = float2(
		(gin[0].posW.x + halfWidth) / terrain_width,
		(gin[0].posW.z + halfDepth) / terrain_depth
		);

	const float2 tiledTexcoord = heightmapTexcoord * tiledTextureScale;

	GOut gout;
	gout.normalW = float3(0.0f, 0.0f, -1.0f); // TODO
	gout.noiseVal = gin[0].noiseVal;
	//gout.baseColour = terrain_tiledTexture.SampleLevel(terrain_tiledTextureSampler, tiledTexcoord, 0);

	float terrainHeight = terrain_heightmap.SampleLevel(terrain_heightmapSampler, heightmapTexcoord, 0).r;

	float3 windFactor = float3(0.07f, 0.0f, 0.0f) * sin(globalTime * 2.0f);

	[unroll]
	for (uint segment = 0; segment < SEGMENTS; ++segment) {
		uint base = segment * 2;
		emit(gin, gout, triStream, base + 0, terrainHeight, windFactor * 0.0f);
		emit(gin, gout, triStream, base + 2, terrainHeight, windFactor * 1.0f);
		emit(gin, gout, triStream, base + 1, terrainHeight, windFactor * 0.0f);
		emit(gin, gout, triStream, base + 1, terrainHeight, windFactor * 0.0f);
		emit(gin, gout, triStream, base + 2, terrainHeight, windFactor * 1.0f);
		emit(gin, gout, triStream, base + 3, terrainHeight, windFactor * 1.0f);
	}
}
