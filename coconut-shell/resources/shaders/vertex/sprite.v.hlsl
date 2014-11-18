cbuffer WorldTransformations {
	matrix worldMatrix;
}

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	pos.w = 1.0f;
	return mul(pos, worldMatrix);
}
