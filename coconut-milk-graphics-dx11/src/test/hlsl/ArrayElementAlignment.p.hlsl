struct SmallStruct {
	float f;
};

struct BigStruct {
	float4 f4;
	float f;
};

struct WithArray {
	BigStruct bigStructs[2];
};

cbuffer buf: register(b0) {
	SmallStruct smallStructs[3];
	float floats[5];
	BigStruct bigStructs[2];
	WithArray withArray;
};

bool almostEqual(float lhs, float rhs) {
	return abs(lhs - rhs) < 0.001f;
}

float4 main() : SV_TARGET {
	if (!almostEqual(smallStructs[0].f, 0.01f)) {
		return float4(0.01f, 0.0f, 0.0f, 0.0f);
	}

	if (!almostEqual(smallStructs[1].f, 0.02f)) {
		return float4(0.02f, 0.0f, 0.0f, 0.0f);
	}

	if (!almostEqual(smallStructs[2].f, 0.03f)) {
		return float4(0.03f, 0.0f, 0.0f, 0.0f);
	}

	if (!almostEqual(floats[0], 0.04f)) {
		return float4(0.04f, 0.0f, 0.0f, 0.0f);
	}

	if (!almostEqual(floats[1], 0.05f)) {
		return float4(0.05f, 0.0f, 0.0f, 0.0f);
	}

	if (!almostEqual(floats[2], 0.06f)) {
		return float4(0.06f, 0.0f, 0.0f, 0.0f);
	}

	if (!almostEqual(floats[3], 0.07f)) {
		return float4(0.07f, 0.0f, 0.0f, 0.0f);
	}

	if (!almostEqual(floats[4], 0.08f)) {
		return float4(0.08f, 0.0f, 0.0f, 0.0f);
	}

	if (!almostEqual(bigStructs[0].f4.x, 0.09f)) {
		return float4(0.09f, 0.0f, 0.0f, 0.0f);
	}

	if (!almostEqual(bigStructs[1].f4.x, 0.10f)) {
		return float4(0.10f, 0.0f, 0.0f, 0.0f);
	}

	if (!almostEqual(withArray.bigStructs[0].f4.x, 0.11f)) {
		return float4(0.11f, 0.0f, 0.0f, 0.0f);
	}

	if (!almostEqual(withArray.bigStructs[1].f4.x, 0.12f)) {
		return float4(0.12f, 0.0f, 0.0f, 0.0f);
	}

	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}
