struct VertexInOut {
	float3 posW : POSITION;
	float2 heightmapTexcoord : TEXCOORD;
};

struct PatchTesselation {
	float edgeTesselation[4] : SV_TessFactor;
	float insideTesselation[2] : SV_InsideTessFactor;
};

struct HullOut {
	float3 posW : POSITION;
	float2 heightmapTexcoord : TEXCOORD;
};

struct DomainOut {
	float4 posH : SV_POSITION;
	float3 posW : POSITION;
	float2 heightmapTexcoord : TEXCOORD;
};
