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

cbuffer SceneData {
	float3 eye;
	uint directionalLightsCount;
	DirectionalLight directionalLights[3];
	uint pointLightsCount;
	PointLight pointLights[3];
}

cbuffer ObjectData {
}

cbuffer GroupData {
	Material material;
}

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
	float3 normalW = float3(0.0f, 1.0f, 0.0f); // TODO

	float3 toEye = normalize(eye - pin.posW);

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

	[unroll]
	for (uint pi = 0; pi < pointLightsCount; ++pi) {
		float4 ambientComp, diffuseComp, specularComp;
		computePoint(material, pointLights[pi], pin.posW, normalW, toEye, ambientComp, diffuseComp, specularComp);

		ambient += ambientComp;
		diffuse += diffuseComp;
		specular += specularComp;
	}

	float4 endColour = saturate(ambient + diffuse + specular);
	endColour.a = 1.0f;

	return endColour;
}
