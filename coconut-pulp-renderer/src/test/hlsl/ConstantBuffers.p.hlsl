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

bool almostEqual(float lhs, float rhs) {
	return abs(lhs - rhs) < 0.001f;
}

bool almostEqual(float3 lhs, float3 rhs) {
	return almostEqual(lhs.x, rhs.x) && almostEqual(lhs.y, rhs.y) && almostEqual(lhs.z, rhs.z);
}

bool almostEqual(float4 lhs, float4 rhs) {
	return almostEqual(lhs.xyz, rhs.xyz) && almostEqual(lhs.w, rhs.w);
}

float4 main(float4 pos : SV_POSITION) : SV_TARGET {
	if (!almostEqual(material.ambientColour, float4(0.01f, 0.02f, 0.03f, 0.04f))) {
		return float4(0.01f, 0.01f, 0.01f, 0.01f);
	}

	if (!almostEqual(material.diffuseColour, float4(0.05f, 0.06f, 0.07f, 0.08f))) {
		return float4(0.02f, 0.02f, 0.02f, 0.02f);
	}
	
	if (!almostEqual(material.specularColour, float4(0.09f, 0.10f, 0.11f, 0.12f))) {
		return float4(0.03f, 0.03f, 0.03f, 0.03f);
	}
	
	if (!almostEqual(eye, float3(0.13f, 0.14f, 0.15f))) {
		return float4(0.04f, 0.04f, 0.04f, 0.04f);
	}

	if (directionalLightsCount != 2) {
		return float4(0.05f, 0.05f, 0.05f, 0.05f);
	}

	if (!almostEqual(directionalLights[0].ambientColour, float4(0.16f, 0.17f, 0.18f, 0.19f))) {
		return float4(0.06f, 0.06f, 0.06f, 0.06f);
	}

	if (!almostEqual(directionalLights[0].diffuseColour, float4(0.20f, 0.21f, 0.22f, 0.23f))) {
		return float4(0.07f, 0.07f, 0.07f, 0.07f);
	}

	if (!almostEqual(directionalLights[0].specularColour, float4(0.24f, 0.25f, 0.26f, 0.27f))) {
		return float4(0.08f, 0.08f, 0.08f, 0.08f);
	}

	if (!almostEqual(directionalLights[0].direction, float3(0.28f, 0.29f, 0.30f))) {
		return float4(0.09f, 0.09f, 0.09f, 0.09f);
	}

	if (!almostEqual(directionalLights[1].ambientColour, float4(0.31f, 0.32f, 0.33f, 0.34f))) {
		return float4(0.1f, 0.1f, 0.1f, 0.1f);
	}

	if (!almostEqual(directionalLights[1].diffuseColour, float4(0.35f, 0.36f, 0.37f, 0.38f))) {
		return float4(0.11f, 0.11f, 0.11f, 0.11f);
	}

	if (!almostEqual(directionalLights[1].specularColour, float4(0.39f, 0.40f, 0.41f, 0.42f))) {
		return float4(0.12f, 0.12f, 0.12f, 0.12f);
	}

	if (!almostEqual(directionalLights[1].direction, float3(0.43f, 0.44f, 0.45f))) {
		return float4(0.13f, 0.13f, 0.13f, 0.13f);
	}

	if (pointLightsCount != 1) {
		return float4(0.14f, 0.14f, 0.14f, 0.14f);
	}

	if (!almostEqual(pointLights[0].position, float3(0.46f, 0.47f, 0.48f))) {
		return float4(0.15f, 0.15f, 0.15f, 0.15f);
	}

	if (!almostEqual(pointLights[0].attenuation, float3(0.49f, 0.50f, 0.51f))) {
		return float4(0.16f, 0.16f, 0.16f, 0.16f);
	}

	if (!almostEqual(pointLights[0].ambientColour, float4(0.52f, 0.53f, 0.54f, 0.55f))) {
		return float4(0.17f, 0.17f, 0.17f, 0.17f);
	}

	if (!almostEqual(pointLights[0].diffuseColour, float4(0.56f, 0.57f, 0.58f, 0.59f))) {
		return float4(0.18f, 0.18f, 0.18f, 0.18f);
	}

	if (!almostEqual(pointLights[0].specularColour, float4(0.60f, 0.61f, 0.62f, 0.63f))) {
		return float4(0.19f, 0.19f, 0.19f, 0.19f);
	}

	return float4(0.0f, 1.0f, 0.0f, 1.0f);
}
