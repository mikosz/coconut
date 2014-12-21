cbuffer WorldTransformations {
	matrix worldMatrix;
}

cbuffer ViewTransformations {
	matrix viewMatrix;
}

cbuffer ProjectionTransformations {
	matrix projectionMatrix;
}

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	pos.w = 1.0f;
	return mul(mul(mul(pos, worldMatrix), viewMatrix), projectionMatrix);
}
