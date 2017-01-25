struct Sub {
	uint ui;
};

struct S {
	Sub s;
	float f;
};

cbuffer buf: register(b0) {
	S s;
};

float4 main(float4 pos : POSITION) : SV_POSITION {
	if (float(s.s.ui) < s.f) {
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	return pos;
}
