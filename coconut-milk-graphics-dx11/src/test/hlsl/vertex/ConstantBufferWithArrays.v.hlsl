struct S {
	float4x4 m;
};

cbuffer buf: register(b0) {
	S s[2];
	float f;
};

float4 main(float4 pos : POSITION) : SV_POSITION {
	if (float(s[0].m[0][0]) < f) {
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	return pos;
}
