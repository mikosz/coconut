cbuffer Material : register(b0) {
	float4 specularColour;
}

float4 main() : SV_TARGET
{
	return specularColour;
}
