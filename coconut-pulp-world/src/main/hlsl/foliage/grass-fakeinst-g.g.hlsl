cbuffer ViewTransformations : register(b0) {
	matrix viewMatrix;
}

cbuffer ProjectionTransformations : register(b1) {
	matrix projectionMatrix;
}

struct GIn {
	float3 posW : POSITION;
	float height : BLADE_HEIGHT; // TODO: use this!
};

struct GOut {
	float4 posH : SV_POSITION;
	float2 tex : TEXCOORD;
	float3 posW : POSITION;
	float3 normalW : NORMAL;
};

//static const float3 verts[] = {
//	float3(-0.003f, 0.0f, 0.0f),
//	float3(0.003f, 0.0f, 0.0f),
//	float3(-0.003f, 0.15f, 0.0f),
//	float3(0.003f, 0.15f, 0.0f),
//};

static const float y_step = 0.15f * 0.25f;
static const float3 verts[] = {
	float3(-0.003f, 0.0f, 0.0f),
	float3(0.003f, 0.0f, 0.0f),
	float3(-0.003f, y_step, 0.0f),
	float3(0.003f, y_step, 0.0f),
	float3(-0.003f, 2.0f * y_step, 0.0f),
	float3(0.003f, 2.0f * y_step, 0.0f),
	float3(-0.003f, 3.0f * y_step, 0.0f),
	float3(0.003f, 3.0f * y_step, 0.0f),
	float3(-0.003f, 4.0f * y_step, 0.0f),
	float3(0.003f, 4.0f * y_step, 0.0f),
};

void emit(GIn gin, inout GOut gout, inout TriangleStream<GOut> triStream, uint index) {
	gout.posW = gin.posW + verts[index];
	gout.posH = mul(mul(float4(gout.posW, 1.0f), viewMatrix), projectionMatrix);
	triStream.Append(gout);
}

[maxvertexcount(24)]
void main(point GIn gin[1], inout TriangleStream<GOut> triStream) {
	GOut gout;
	gout.normalW = float3(0.0f, 0.0f, -1.0f);
	gout.tex = float2(0.0f, 0.0f);

	[unroll]
	for (uint segment = 0; segment < 4; ++segment) {
		uint base = segment * 2;
		emit(gin, gout, triStream, base + 0);
		emit(gin, gout, triStream, base + 2);
		emit(gin, gout, triStream, base + 1);
		emit(gin, gout, triStream, base + 1);
		emit(gin, gout, triStream, base + 2);
		emit(gin, gout, triStream, base + 3);
	}
}
