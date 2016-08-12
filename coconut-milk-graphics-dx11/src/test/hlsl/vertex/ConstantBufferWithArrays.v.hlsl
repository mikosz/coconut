struct Sub {
	uint ui;
};

struct S {
	Sub s[2];
	float f;
};

cbuffer buf: register(b0) {
	S s[2];
};

float4 main(float4 pos : POSITION) : SV_POSITION {
	if (float(s[0].s[1].ui) < s[1].f) {
		return float4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	return pos;
}
