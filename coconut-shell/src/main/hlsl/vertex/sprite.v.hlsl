cbuffer WorldTransformations : register(b0) {
	matrix worldMatrix;
}

cbuffer ViewTransformations : register(b1) {
	matrix viewMatrix;
}

cbuffer ProjectionTransformations : register(b2) {
	matrix projectionMatrix;
}

float4 main(float4 pos : POSITION) : SV_POSITION
{
	pos.w = 1.0f;
	return mul(mul(mul(pos, worldMatrix), viewMatrix), projectionMatrix);
}
