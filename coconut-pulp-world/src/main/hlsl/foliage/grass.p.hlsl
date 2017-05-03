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

struct SceneData {
	float3 cameraPosition;
	uint directionalLightsCount;
	DirectionalLight directionalLights[3];
	uint pointLightsCount;
	PointLight pointLights[3];
};

cbuffer scene_ {
	SceneData scene;
}

cbuffer ObjectData {
}

cbuffer GroupData {
	Material material;
}

struct PIn {
	float4 posH : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
};

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

float4 main(PIn pin) : SV_TARGET
{
	pin.normalW = normalize(pin.normalW);

	float3 toEye = normalize(scene.cameraPosition - pin.posW);

	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

	[unroll]
	for (uint di = 0; di < scene.directionalLightsCount; ++di) {
		float4 ambientComp, diffuseComp, specularComp;
		computeDirectional(material, scene.directionalLights[di], pin.normalW, toEye, ambientComp, diffuseComp, specularComp);

		ambient += ambientComp;
		diffuse += diffuseComp;
		specular += specularComp;
	}
	
	[unroll]
	for (uint pi = 0; pi < scene.pointLightsCount; ++pi) {
		float4 ambientComp, diffuseComp, specularComp;
		computePoint(material, scene.pointLights[pi], pin.posW, pin.normalW, toEye, ambientComp, diffuseComp, specularComp);

		ambient += ambientComp;
		diffuse += diffuseComp;
		specular += specularComp;
	}

	float4 endColour = saturate(ambient + diffuse + specular);
	endColour.a = 1.0f;
	
	return endColour;
}
