struct VIn {
	float4 posL : POSITION;
	float2 tex : TEXCOORD0;
	float2 tex2 : TEXCOORD1;
	float3 normalL : NORMAL;
};

float4 main(VIn vin) : SV_POSITION {
	return vin.posL;
}
