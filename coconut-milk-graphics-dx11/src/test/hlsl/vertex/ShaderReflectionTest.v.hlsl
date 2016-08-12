

struct VIn {
	float4 posL : POSITION;
	float2 tex : TEXCOORD;
	float3 normalL : NORMAL;
};

struct VOut {
	float4 posH : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
};

VOut main(VIn vin) {
	VOut result;

	result.posH = float4(0.0f, 0.0f, 0.0f, 0.0f);
	result.tex = float2(0.0f, 0.0f);
	result.posW = float3(0.0f, 0.0f, 0.0f);
	result.normalW = float3(0.0f, 0.0f, 0.0f);

	if (ui < s.ui) { // use data from cbuffers to avoid them being optimised-out
		result.posH = vin.posL + float4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	return result;
}
