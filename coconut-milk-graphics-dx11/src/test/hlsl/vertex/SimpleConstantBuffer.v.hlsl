cbuffer buf: register(b0) {
	uint ui;
	float4 f4;
	float4x4 m4;
};

float4 main(float4 pos : POSITION) : SV_POSITION {
	if (float(ui) > 10.0f) {
		return f4 + m4[0];
	} else {
		return f4;
	}
}
