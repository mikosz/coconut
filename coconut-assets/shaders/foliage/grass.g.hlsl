#include "grass-common.hlsl"

static const uint GS_INPUT_VERTEX_COUNT = 1;
static const float WIND_OSCILLATION_AMPLITUDE = 0.05f;
static const float2 WIND_FORCE_RANDOM_AMPLITUDE = { 0.7f, 1.0f };
static const float2 WIND_DIRECTION_RANDOM_AMPLITUDE = { 0.0f, 0.025f };
static const float HEIGHT_SKEW_AMPLITUDE = 0.1f;
static const float BLADE_WIDTH = 0.01f;
static const float BLADE_HEIGHT = 0.4f;
static const int SEGMENT_COUNT_PER_LOD[3] = { 5, 3, 2 };
static const float LOD_SEGMENT_WIND_COEFFICIENT[3][5] = {
	{ 0.0, 0.15, 0.35, 0.6, 0.9 },
	{ 0.0, 0.35, 0.9, 0.0, 0.0 },
	{ 0.0, 0.9, 0.0, 0.0, 0.0 }								
	};

typedef int Side;
static const Side LEFT = -1;
static const Side RIGHT = 1;
	
struct SceneData {
	matrix view;
	matrix projection;
	float3 cameraPosition;
};

cbuffer SceneBuffer {
	SceneData scene;
    float globalTime;
};
	
struct BladeParams {
    float3 faceDir;
	float3 rootPosW;
    float randomSeed;
    float heightSkew;
    float heightScale;
};
	
void randomiseWindForce(inout float2 windDir, float randomSeed) {
    windDir *= lerp(WIND_FORCE_RANDOM_AMPLITUDE.x, WIND_FORCE_RANDOM_AMPLITUDE.y, 1.0f - randomSeed);
}

void oscillateWind(inout float2 windDir, float randomSeed) {
    const float amount = (sin(7.0f * globalTime + randomSeed) + 1.0f) / 2.0f;
    const float2 leftWindBound = windDir * (1.0 - WIND_OSCILLATION_AMPLITUDE);
    const float2 rightWindBound = windDir * (1.0 + WIND_OSCILLATION_AMPLITUDE);
    windDir = lerp(leftWindBound, rightWindBound, amount);
}

void randomiseWindDirection(inout float2 windDir, float randomSeed) {
    const float randomAngle = lerp(-3.14f, 3.14f, randomSeed);
	const float2 randomWindDir = float2(sin(randomAngle), cos(randomAngle));
	const float randomForce = lerp(WIND_DIRECTION_RANDOM_AMPLITUDE.x, WIND_DIRECTION_RANDOM_AMPLITUDE.y, randomSeed);
	windDir += randomWindDir * randomForce;
}

PIn getBladeVertex(BladeParams blade, uint lod, uint segment, Side side) {
	PIn result = (PIn) 0;
	
	const int segmentCount = SEGMENT_COUNT_PER_LOD[lod];
	const float segmentHeightNorm = float(segment) / (segmentCount - 1);
	
    const float bladeHeight = blade.heightScale * (BLADE_HEIGHT + blade.heightSkew);
    const float bladeWidth = BLADE_WIDTH * lerp(2.0f, 1.0f, blade.heightScale);

	result.posW = blade.rootPosW;
	result.posW.y += segmentHeightNorm * bladeHeight;
	result.posW.xyz += blade.faceDir * side * bladeWidth * 0.5f;
	
	const float windCoefficient = LOD_SEGMENT_WIND_COEFFICIENT[lod][segment];
	float2 windDir = float2(0.1f, 0.1f);
	
    randomiseWindForce(windDir, blade.randomSeed);
    oscillateWind(windDir, blade.randomSeed);
    randomiseWindDirection(windDir, blade.randomSeed);

    result.posW.y += -windCoefficient * (length(windDir) * 0.5);
	result.posW.xz += windDir.xy * windCoefficient;

	result.tex.x = (side + 1.0f) * 0.5f;
	result.tex.y = 1.0f - segmentHeightNorm;
	
	return result;
}
	
void generateNormal(inout PIn v0, inout PIn v1, inout PIn v2) {
	float3 side0 = normalize(v0.posW.xyz - v1.posW.xyz);
	float3 side1 = normalize(v2.posW.xyz - v1.posW.xyz);
	
	float3 normal = normalize(cross(side0, side1));
	
	v0.normalW += normal;
	v1.normalW += normal;
	v2.normalW += normal;
}

void prepareForPS(inout PIn v, GIn root) {
	v.posH = mul(mul(float4(v.posW, 1.0f), scene.view), scene.projection);
    v.noiseVal = root.noiseVal;
}
	
void buildBlade(GIn root, uint lod, inout TriangleStream<PIn> triangles) {
	uint i = 0;
	
	const uint segmentCount = SEGMENT_COUNT_PER_LOD[0 /*lod*/]; // TODO: fixme: won't work, cause lod not literal
	const uint vertexCount = segmentCount * 2;
	
	PIn vertices[vertexCount];
	
    const float distanceToCamera = length(root.posW - scene.cameraPosition);
    const float heightScale = 1.0f - clamp((distanceToCamera - 20.0f) / 20.0f, 0.0f, 1.0f);

	BladeParams bladeParams = {
        { sin(root.posW.y + root.noiseVal), 0.0f, cos(root.posW.y + root.noiseVal) }, // face direction // TODO: bad name, bad usage
		root.posW,
        root.noiseVal,
        HEIGHT_SKEW_AMPLITUDE * cos(root.posW.x + root.posW.y), // heightSkew
        heightScale
		};

	for (i = 0; i < segmentCount; ++i) {
		vertices[i * 2] = getBladeVertex(bladeParams, 0, i, LEFT);
		vertices[i * 2 + 1] = getBladeVertex(bladeParams, 0, i, RIGHT);
	}

	for (i = 0; i < segmentCount - 1; ++i) {	
		uint vertexIdx = i * 2;
		generateNormal(vertices[vertexIdx + 0], vertices[vertexIdx + 1], vertices[vertexIdx + 2]);
		generateNormal(vertices[vertexIdx + 1], vertices[vertexIdx + 3], vertices[vertexIdx + 2]);
	}
	
	for (i = 0; i < vertexCount; ++i) {	
		prepareForPS(vertices[i], root);
		triangles.Append(vertices[i]);
	}
}
	
// TODO: have different main functions per LOD, or accept LOD through cbuffer, either way
// LODs are implemented but not used (0 used below)
[maxvertexcount(SEGMENT_COUNT_PER_LOD[0] * 2 * GS_INPUT_VERTEX_COUNT)]
void main(
	point GIn input[GS_INPUT_VERTEX_COUNT], // TODO: Eddie Lee uses triangles, why? Quicker?
	inout TriangleStream<PIn> triangles
	)
{
	[unroll]
	for (uint i = 0; i < GS_INPUT_VERTEX_COUNT; ++i)
	{
		buildBlade(input[i], 0, triangles);
		triangles.RestartStrip();
	}
}
