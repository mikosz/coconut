struct GIn {
	float3 posW : POSITION;
	float noiseVal : NOISE;
};

struct PIn {
	float4 posH : SV_POSITION;
	float3 posW : POSITION;
	float noiseVal : NOISE;
	float3 normalW : NORMAL;
	float2 tex : TEXCOORD;
};
