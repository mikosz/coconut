#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

#include <DirectXMath.h>

#include <coconut-tools/utils/Range.hpp>

#include "coconut/pulp/math/Transform.hpp"

using namespace coconut;
using namespace coconut::pulp;
using namespace coconut::pulp::math;

namespace /* anonymous */ {

BOOST_AUTO_TEST_SUITE(PulpTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathTestSuite);
BOOST_AUTO_TEST_SUITE(PulpMathTransformTestSuite);

BOOST_AUTO_TEST_CASE(ConstructsValidOrthographicProjection) {
	const auto orthoLH = Transform::orthographicProjection(Handedness::LEFT, -5.0f, 5.0f, 2.5f, -2.5f, -0.1f, 100.0f);
	auto expectedLH = DirectX::XMFLOAT4X4();
	DirectX::XMStoreFloat4x4(&expectedLH, DirectX::XMMatrixOrthographicOffCenterLH(-5.0f, 5.0f, -2.5f, 2.5f, -0.1f, 100.0f));

	const auto orthoRH = Transform::orthographicProjection(Handedness::RIGHT, -5.0f, 5.0f, 2.5f, -2.5f, -0.1f, 100.0f);
	auto expectedRH = DirectX::XMFLOAT4X4();
	DirectX::XMStoreFloat4x4(&expectedRH, DirectX::XMMatrixOrthographicOffCenterRH(-5.0f, 5.0f, -2.5f, 2.5f, -0.1f, 100.0f));

	for (const auto row : coconut_tools::range(0u, 4u)) {
		for (const auto column : coconut_tools::range(0u, 4u)) {
			BOOST_CHECK_EQUAL(orthoLH.matrix()[row][column], expectedLH(row, column));
			BOOST_CHECK_EQUAL(orthoRH.matrix()[row][column], expectedRH(row, column));
		}
	}
}

BOOST_AUTO_TEST_CASE(ConstructsValidPerspectiveProjection) {
	const auto perspectiveLH = Transform::perspectiveProjection(Handedness::LEFT, 0.25_rad, 0.75f, -0.1f, 100.0f);
	auto expectedLH = DirectX::XMFLOAT4X4();
	DirectX::XMStoreFloat4x4(&expectedLH, DirectX::XMMatrixPerspectiveFovLH(0.25f, 0.75f, -0.1f, 100.0f));

	const auto perspectiveRH = Transform::perspectiveProjection(Handedness::RIGHT, 0.25_rad, 0.75f, -0.1f, 100.0f);
	auto expectedRH = DirectX::XMFLOAT4X4();
	DirectX::XMStoreFloat4x4(&expectedRH, DirectX::XMMatrixPerspectiveFovRH(0.25f, 0.75f, -0.1f, 100.0f));

	for (const auto row : coconut_tools::range(0u, 4u)) {
		for (const auto column : coconut_tools::range(0u, 4u)) {
			BOOST_CHECK_EQUAL(perspectiveLH.matrix()[row][column], expectedLH(row, column));
			BOOST_CHECK_EQUAL(perspectiveRH.matrix()[row][column], expectedRH(row, column));
		}
	}
}

BOOST_AUTO_TEST_CASE(ConstructsValidTranslation) {
	auto translation = Transform::translation({ 1.0f, 2.0f, 3.0f });

	auto expected = DirectX::XMFLOAT4X4();
	DirectX::XMStoreFloat4x4(&expected, DirectX::XMMatrixTranslation(1.0f, 2.0f, 3.0f));
	
	for (const auto row : coconut_tools::range(0u, 4u)) {
		for (const auto column : coconut_tools::range(0u, 4u)) {
			BOOST_CHECK_EQUAL(translation.matrix()[row][column], expected(row, column));
		}
	}

	const auto start = Vec4(2.0f, 0.2f, 1.0f, 1.0f);
	const auto end = Vec4(3.0f, 2.2f, 4.0f, 1.0f);
	const auto translated = translation.apply(start);
	BOOST_CHECK_EQUAL(translated, end);
}

BOOST_AUTO_TEST_CASE(ConstructsValidScale) {
	auto scale = Transform::scale({ 1.0f, 2.0f, 3.0f });

	auto expected = DirectX::XMFLOAT4X4();
	DirectX::XMStoreFloat4x4(&expected, DirectX::XMMatrixScaling(1.0f, 2.0f, 3.0f));

	for (const auto row : coconut_tools::range(0u, 4u)) {
		for (const auto column : coconut_tools::range(0u, 4u)) {
			BOOST_CHECK_EQUAL(scale.matrix()[row][column], expected(row, column));
		}
	}

	const auto start = Vec4(2.0f, 0.2f, 1.0f, 1.0f);
	const auto end = Vec4(2.0f, 0.4f, 3.0f, 1.0f);
	const auto scaled = scale.apply(start);
	BOOST_CHECK_EQUAL(scaled, end);
}

BOOST_AUTO_TEST_CASE(ConstructsValidRotation) {
	auto rotation = Transform::rotation(Vec3(2.0f, 1.0f, 3.0f).normalised(), -23.0_deg);

	auto expected = DirectX::XMFLOAT4X4();
	const auto axis = DirectX::XMFLOAT3(2.0f, 1.0f, 3.0f);
	DirectX::XMStoreFloat4x4(
		&expected,
		DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&axis), -23.0_deg.radians())
		);

	for (const auto row : coconut_tools::range(0u, 4u)) {
		for (const auto column : coconut_tools::range(0u, 4u)) {
			BOOST_CHECK_EQUAL(rotation.matrix()[row][column], expected(row, column));
		}
	}

	const auto start = Vec4(1.0f, 3.0f, 5.0f, 1.0f);
	const auto end = Vec4(1.565343f, 3.60607f, 4.421081f, 1.0f);
	const auto rotated = rotation.apply(start);
	BOOST_CHECK_EQUAL(rotated, end);
}

BOOST_AUTO_TEST_SUITE_END(/* PulpMathTransformTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpMathTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpTestSuite */);

} // namespace anonymous
