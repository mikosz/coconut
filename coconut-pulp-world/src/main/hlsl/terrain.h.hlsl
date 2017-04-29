#include "terrain.hlsl"

cbuffer Settings {
	float terrainMinTesselationDistance;
	float terrainMaxTesselationDistance;
	float terrainMinTesselationExponent;
	float terrainMaxTesselationExponent;
};

cbuffer Context {
	float3 eye;
};

float calculateTesselation(float3 controlPoint) {
	float d = distance(controlPoint, eye);
	float s = saturate((d - terrainMinTesselationDistance) /
		(terrainMaxTesselationDistance - terrainMinTesselationDistance));
	return pow(2, lerp(terrainMaxTesselationExponent, terrainMinTesselationExponent, s));
}

/**
 * Assuming patch layout:
 * 0 - 1
 * |   |
 * 2 - 3
 */
PatchTesselation constantHS(InputPatch<VertexInOut, 4> patch, uint patchID : SV_PrimitiveID) {
	float3 edgeMidpoint_0_2 = 0.5f * (patch[0].posW + patch[2].posW);
	float3 edgeMidpoint_0_1 = 0.5f * (patch[0].posW + patch[1].posW);
	float3 edgeMidpoint_1_3 = 0.5f * (patch[1].posW + patch[3].posW);
	float3 edgeMidpoint_2_3 = 0.5f * (patch[2].posW + patch[3].posW);
	
	float3 centre = 0.25f * (patch[0].posW + patch[1].posW + patch[2].posW + patch[3].posW);

	PatchTesselation pt;
	
	pt.edgeTesselation[0] = calculateTesselation(edgeMidpoint_0_2);
	pt.edgeTesselation[1] = calculateTesselation(edgeMidpoint_0_1);
	pt.edgeTesselation[2] = calculateTesselation(edgeMidpoint_1_3);
	pt.edgeTesselation[3] = calculateTesselation(edgeMidpoint_2_3);

	pt.insideTesselation[0] = calculateTesselation(centre);
	pt.insideTesselation[1] = pt.insideTesselation[0];

	return pt;
}

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("constantHS")]
[maxtessfactor(64.0f)]
HullOut main(
	InputPatch<VertexInOut, 4> patch,
	uint controlPointId : SV_OutputControlPointID,
	uint patchId : SV_PrimitiveID
	)
{
	HullOut hout;
	hout.posW = patch[controlPointId].posW;
	hout.heightmapTexcoord = patch[controlPointId].heightmapTexcoord;

	return hout;
}
