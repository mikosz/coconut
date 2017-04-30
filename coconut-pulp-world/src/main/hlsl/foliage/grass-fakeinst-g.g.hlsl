cbuffer ViewTransformations {
	matrix viewMatrix;
}

cbuffer ProjectionTransformations {
	matrix projectionMatrix;
}

cbuffer TerrainData {
	float terrainCellEdgeLength;
	float terrainWidth;
	float terrainDepth;
};

struct GIn {
	float3 posW : POSITION;
	float yScale : BLADE_HEIGHT_SCALE;
};

struct GOut {
	float4 posH : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
};

Texture2D heightmap;
SamplerState heightmapSampler;

static const float3 verts[] = {
	float3(-0.003f, 0.0f, 0.0f),
	float3(0.003f, 0.0f, 0.0f),
	float3(-0.003f, 0.15f, 0.0f),
	float3(0.003f, 0.15f, 0.0f),
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

void emit(GIn gin, inout GOut gout, inout TriangleStream<GOut> triStream, uint index, float terrainHeight) {
	gout.posW = gin.posW + verts[index];
	gout.posW.y *= gin.yScale;
	gout.posW.y += terrainHeight;
	gout.posH = mul(mul(float4(gout.posW, 1.0f), viewMatrix), projectionMatrix);
	triStream.Append(gout);
}

[maxvertexcount(6 * SEGMENTS)]
void main(point GIn gin[1], inout TriangleStream<GOut> triStream) {
	GOut gout;
	gout.normalW = float3(0.0f, 0.0f, -1.0f);
	gout.tex = float2(0.0f, 0.0f);

	const float halfWidth = terrainWidth * 0.5f;
	const float halfDepth = terrainDepth * 0.5f;
	const float2 heightmapTexcoord = float2(
		(gin[0].posW.x + halfWidth) / terrainWidth,
		(gin[0].posW.z + halfDepth) / terrainDepth
		);

	float terrainHeight = heightmap.SampleLevel(heightmapSampler, heightmapTexcoord, 0).r;

	[unroll]
	for (uint segment = 0; segment < SEGMENTS; ++segment) {
		uint base = segment * 2;
		emit(gin, gout, triStream, base + 0, terrainHeight);
		emit(gin, gout, triStream, base + 2, terrainHeight);
		emit(gin, gout, triStream, base + 1, terrainHeight);
		emit(gin, gout, triStream, base + 1, terrainHeight);
		emit(gin, gout, triStream, base + 2, terrainHeight);
		emit(gin, gout, triStream, base + 3, terrainHeight);
	}
}
