struct GIn {
	float3 posW : POSITION;
	float4 baseColour : COLOR;
	float2 windDir : WIND_DIR;
	float noiseVal : NOISE;
};

struct PIn {
	float4 posH : SV_POSITION;
	float4 baseColour : COLOR;
	float3 posW : POSITION;
	float terrainHeightW : TERRAIN_HEIGHT;
	float3 normalW : NORMAL;
	float noiseVal : NOISE;
	float2 tex : TEXCOORD;
};
