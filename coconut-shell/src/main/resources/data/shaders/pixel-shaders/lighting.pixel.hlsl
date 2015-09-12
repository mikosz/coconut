Texture2D tex[4];
Texture2D reflectionTexture;
SamplerState samplerState;

cbuffer LightBuffer : register(b0) {
	float4 ambientColour;
	float4 diffuseColour;
	float3 lightDirection;
	float specularPower;
	float4 _ /* specularColour */; // ignored
	float gamma;

	float3 padding_;
}

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 textureCoord : TEXCOORD0;
	float3 viewDirection : TEXCOORD1;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float fogFactor : FOG;
	float4 reflectionPosition : TEXCOORD2;
};

float4 main(PixelInputType input) : SV_TARGET
{
    float4 baseTextureColour = tex[0].Sample(samplerState, input.textureCoord);
    float4 detailTextureColour = tex[1].Sample(samplerState, input.textureCoord);
	float4 textureColour = saturate(baseTextureColour * detailTextureColour * gamma);

	float4 bumpMapValue = tex[2].Sample(samplerState, input.textureCoord);
	// Expand the range of the normal value from (0, +1) to (-1, +1)
	bumpMapValue = (bumpMapValue * 2.0f) - 1.0f;

	float3 bumpNormal = (bumpMapValue.x * input.tangent) +
		(bumpMapValue.y * input.binormal) +
		(bumpMapValue.z * input.normal);
	bumpNormal = normalize(bumpNormal);

	float4 lightColour = ambientColour;

	float4 specular = 0;

	float lightIntensity = saturate(dot(bumpNormal, -lightDirection));
	if (lightIntensity > 0.0f) {
		lightColour = saturate(lightColour + saturate(diffuseColour * lightIntensity));

		float3 reflection = normalize(2 * lightIntensity * bumpNormal - lightDirection);
		float4 specularColour = tex[3].Sample(samplerState, input.textureCoord);
		specular = specularColour * pow(saturate(dot(reflection, input.viewDirection)), specularPower);
	}

	float4 litColour = saturate(lightColour * textureColour + specular);

	float4 fogColour = float4(0.5f, 0.2f, 0.2f, 1.0f);

	float4 endColour = input.fogFactor * litColour + (1.0f - input.fogFactor) * fogColour;

	float2 reflectTexCoord;
	reflectTexCoord.x = input.reflectionPosition.x / input.reflectionPosition.w / 2.0f + 0.5f;
	reflectTexCoord.y = input.reflectionPosition.y / input.reflectionPosition.w / 2.0f + 0.5f;

	float4 reflectionColour = reflectionTexture.Sample(samplerState, reflectTexCoord);

	return lerp(endColour, reflectionColour, 0.15f);
}
