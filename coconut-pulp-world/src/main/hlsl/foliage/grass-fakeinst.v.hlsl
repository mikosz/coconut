struct SceneData {
	matrix view;
	matrix projection;
};

cbuffer SceneBuffer {
	SceneData scene;
};

cbuffer PatchData {
	float3 grassPatchPosition;
};

//static const float3 verts[] = {
//	float3(-0.003f, 0.0f, 0.0f),
//	float3(0.003f, 0.0f, 0.0f),
//	float3(-0.003f, 0.15f, 0.0f),
//	float3(0.003f, 0.15f, 0.0f),
//};
//static const uint verts_count = 4;

static const float y_step = 0.15f * 0.25f;

static const float3 verts[] = {
	float3(-0.003f, 0.0f, 0.0f),
	float3(0.003f, 0.0f, 0.0f),
	float3(-0.003f, y_step, 0.0f),
	float3(0.003f, y_step, 0.0f),
	float3(-0.003f, 2.0f * y_step, 0.0f),
	float3(0.003f, 2.0f * y_step, 0.0f),
	float3(-0.003f, 3.0f * y_step, 0.0f),
	float3(0.003f, 3.0f * y_step, 0.0f),
	float3(-0.003f, 4.0f * y_step, 0.0f),
	float3(0.003f, 4.0f * y_step, 0.0f),
};
static const uint verts_count = 10;

Texture2D noiseMap : register(t0);

struct VOut {
	float4 posH : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
};

VOut main(uint vertexId : SV_VertexID)
{
	static const float OFFSET = 0.05f;
	static const uint BLADES_PER_ROW = 800;
	static const float HALF_OFFSET = OFFSET * 0.5f;
	static const uint TEXTURE_WIDTH = 800;
	static const uint TEXTURE_HEIGHT = 600;

	VOut vout;
	
	const uint bladeId = vertexId / verts_count;
	const uint columnId = bladeId % BLADES_PER_ROW;
	const uint rowId = bladeId / BLADES_PER_ROW;
	
	float4 posW = float4(verts[vertexId % verts_count] + grassPatchPosition, 1.0f);
	posW.x += (columnId) * OFFSET;
	posW.z += (rowId) * OFFSET;

	float4 noise = noiseMap[uint2(rowId % TEXTURE_WIDTH, columnId % TEXTURE_HEIGHT)];
	posW.x += noise.x * HALF_OFFSET;
	posW.y *= 1.0f + noise.y;
	posW.z += noise.z * HALF_OFFSET;

	vout.posH = mul(mul(posW, scene.view), scene.projection);
	vout.posW = posW.xyz;
	vout.normalW = float3(0.0f, 0.0f, -1.0f);

	return vout;
}
