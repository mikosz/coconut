#include "grass-common.hlsl"

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

cbuffer ObjectData {
}

cbuffer GroupData {
	Material material;
}

Texture2D material_diffuseMap;
Texture2D material_alphaMap;
Texture2D material_subsurfaceMap;
SamplerState material_diffuseMapSampler;

void computeDirectional(
	Material mat,
	DirectionalLight l,
	float3 normal,
	float3 toEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 specular,
	out float4 subsurface
	) {
	ambient = mat.ambientColour * l.ambientColour;
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	subsurface = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVec = -l.direction;
	float diffuseFactor = dot(lightVec, normal);
	if (diffuseFactor > 0.0f) {
		float3 reflected = reflect(-lightVec, normal);
		float specularFactor = pow(max(dot(reflected, toEye), 0.0f), mat.specularColour.w);

		diffuse = diffuseFactor * mat.diffuseColour * l.diffuseColour;
		specular = specularFactor * mat.specularColour * l.specularColour;
	}

	float subsurfaceFactor = -diffuseFactor;
	if (subsurfaceFactor > 0.0f) {
		subsurface = subsurfaceFactor * mat.diffuseColour * l.diffuseColour;
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

float4 main(PIn pin) : SV_TARGET
{
	pin.normalW = normalize(pin.normalW);

	float3 toEye = normalize(cameraPosition - pin.posW);

	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 subsurface = float4(0.0f, 0.0f, 0.0f, 0.0f);

	[unroll]
	for (uint di = 0; di < directionalLightsCount; ++di) {
		float4 ambientComp, diffuseComp, specularComp, subsurfaceComp;
		computeDirectional(material, directionalLights[di], pin.normalW, toEye, ambientComp, diffuseComp, specularComp, subsurfaceComp);

		ambient += ambientComp;
		diffuse += diffuseComp;
		specular += specularComp;
		subsurface += subsurfaceComp;
	}
	
	//[unroll]
	//for (uint pi = 0; pi < pointLightsCount; ++pi) {
	//	float4 ambientComp, diffuseComp, specularComp;
	//	computePoint(material, pointLights[pi], pin.posW, pin.normalW, toEye, ambientComp, diffuseComp, specularComp);

	//	ambient += ambientComp;
	//	diffuse += diffuseComp;
	//	specular += specularComp;
	//}

	float4 subsurfaceColour = material_subsurfaceMap.Sample(material_diffuseMapSampler, pin.tex);

	static const float4 groundColour = { 0.14f, 0.11f, 0.06f, 1.0f };
	float4 unlitColour = pin.baseColour;

	unlitColour = lerp(groundColour, unlitColour, saturate((pin.posW.y - pin.terrainHeightW) / 0.05f));
	
	float4 endColour = saturate(
		unlitColour * (ambient + diffuse)
		+ subsurfaceColour * subsurface
		+ specular
		);
		
	float alpha = material_alphaMap.Sample(material_diffuseMapSampler, pin.tex).r;
	endColour.a = alpha; // * diffuse.a;
	
	return endColour;
}
