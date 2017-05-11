#include "terrain.hlsl"

struct Material {
	float4 ambientColour;
	float4 diffuseColour;
	float4 specularColour;
};

struct DirectionalLight {
	float4 ambientColour;
	float4 diffuseColour;
	float4 specularColour;
	float3 direction;
};

struct PointLight {
	float3 position;
	float3 attenuation;
	float4 ambientColour;
	float4 diffuseColour;
	float4 specularColour;
};

cbuffer scene {
	float3 cameraPosition;
	uint directionalLightsCount;
	DirectionalLight directionalLights[3];
	//uint pointLightsCount;
	//PointLight pointLights[3];
}

cbuffer MaterialData {
	Material material;
}

cbuffer terrain {
	float cellEdgeLength;
	float width;
	float depth;
};

Texture2D terrain_heightmap;
SamplerState terrain_heightmapSampler;

Texture2D terrain_tiledTexture;
SamplerState terrain_tiledTextureSampler;

void computeDirectional(Material mat, DirectionalLight l, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 specular) {
	ambient = mat.ambientColour * l.ambientColour;
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = -l.direction;
	float diffuseFactor = dot(lightVec, normal);
	if (diffuseFactor > 0.0f) {
		float3 reflected = reflect(-lightVec, normal);
		float specularFactor = pow(max(dot(reflected, toEye), 0.0f), mat.specularColour.w);

		diffuse = diffuseFactor * mat.diffuseColour * l.diffuseColour;
		specular = specularFactor * mat.specularColour * l.specularColour;
	}
}

void computePoint(Material mat, PointLight l, float3 position, float3 normal, float3 toEye, out float4 ambient, out float4 diffuse, out float4 specular) {
	ambient = mat.ambientColour * l.ambientColour;
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float distance = length(l.position - position);
	float3 lightVec = (l.position - position) / distance;
	float diffuseFactor = dot(lightVec, normal);
	if (diffuseFactor > 0.0f) {
		float intensity = 1.0f / dot(l.attenuation, float3(1.0f, distance, distance * distance));

		float3 reflected = reflect(-lightVec, normal);
		float specularFactor = pow(max(dot(reflected, toEye), 0.0f), mat.specularColour.w);

		diffuse = intensity * diffuseFactor * mat.diffuseColour * l.diffuseColour;
		specular = intensity * specularFactor * mat.specularColour * l.specularColour;
	}
}

float4 main(DomainOut pin) : SV_TARGET
{
	const float texelCellSpaceU = cellEdgeLength / width;
	const float texelCellSpaceV = cellEdgeLength / depth;

	const float2 leftUV = pin.heightmapTexcoord + float2(-texelCellSpaceU, 0.0f);
	const float2 rightUV = pin.heightmapTexcoord + float2(texelCellSpaceU, 0.0f);
	const float2 bottomUV = pin.heightmapTexcoord + float2(0.0f, texelCellSpaceV);
	const float2 topUV = pin.heightmapTexcoord + float2(0.0f, -texelCellSpaceV);

	const float leftHeight = terrain_heightmap.SampleLevel(terrain_heightmapSampler, leftUV, 0).r;
	const float rightHeight = terrain_heightmap.SampleLevel(terrain_heightmapSampler, rightUV, 0).r;
	const float bottomHeight = terrain_heightmap.SampleLevel(terrain_heightmapSampler, bottomUV, 0).r;
	const float topHeight = terrain_heightmap.SampleLevel(terrain_heightmapSampler, topUV, 0).r;

	const float3 tangent = normalize(float3(2.0f * cellEdgeLength, rightHeight - leftHeight, 0.0f));
	const float3 bitangent = normalize(float3(0.0f, bottomHeight - topHeight, 2.0f * cellEdgeLength));
	const float3 normalW = cross(bitangent, tangent);

	float3 toEye = normalize(cameraPosition - pin.posW);

	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	[unroll]
	for (uint di = 0; di < directionalLightsCount; ++di) {
		float4 ambientComp, diffuseComp, specularComp;
		computeDirectional(material, directionalLights[di], normalW, toEye, ambientComp, diffuseComp, specularComp);

		ambient += ambientComp;
		diffuse += diffuseComp;
		specular += specularComp;
	}

	//[unroll]
	//for (uint pi = 0; pi < pointLightsCount; ++pi) {
	//	float4 ambientComp, diffuseComp, specularComp;
	//	computePoint(material, pointLights[pi], pin.posW, normalW, toEye, ambientComp, diffuseComp, specularComp);

	//	ambient += ambientComp;
	//	diffuse += diffuseComp;
	//	specular += specularComp;
	//}

	float4 textureColour = terrain_tiledTexture.Sample(terrain_tiledTextureSampler, pin.tiledTexcoord);

	float4 endColour = saturate(textureColour * (ambient + diffuse) + specular);
	endColour.a = diffuse.a;

	return endColour;
}
