#define BOOST_TEST_NO_LIB
#include <boost/test/auto_unit_test.hpp>

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
	const auto expectedLH = Matrix4x4(
		{ 0.2f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.4f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 0.00999f, 0.0f },
		{ 0.0f, 0.0f, 0.000999f, 1.0f }
		);

	BOOST_CHECK_EQUAL(orthoLH.matrix(), expectedLH);

	const auto orthoRH = Transform::orthographicProjection(Handedness::RIGHT, -5.0f, 5.0f, 2.5f, -2.5f, -0.1f, 100.0f);
	const auto expectedRH = Matrix4x4(
		{ 0.2f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 0.4f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, -0.00999f, 0.0f },
		{ 0.0f, 0.0f, 0.000999f, 1.0f }
		);

	BOOST_CHECK_EQUAL(orthoRH.matrix(), expectedRH);
}

BOOST_AUTO_TEST_CASE(OrthographicProjectionWorks) {
	const auto orthographic = Transform::orthographicProjection(Handedness::LEFT, -0.5f, 1.0f, 0.5f, -0.5f, -1.0f, 1.0f);

	BOOST_CHECK_EQUAL(orthographic.apply({ 1.0f, -0.5f, -1.0f, 1.0f }), Vec4(1.0f, -1.0f, 0.0f, 1.0f));
	BOOST_CHECK_EQUAL(orthographic.apply({ -0.5f, 0.5f, 0.0f, 1.0f }), Vec4(-1.0f, 1.0f, 0.5f, 1.0f));
	BOOST_CHECK_EQUAL(orthographic.apply({ 0.0f, 0.0f, 1.0f, 1.0f }), Vec4(-0.333333f, 0.0f, 1.0f, 1.0f));
}

BOOST_AUTO_TEST_CASE(ConstructsExpectedPerspectiveProjectionMatrix) {
	const auto perspectiveLH = Transform::perspectiveProjection(Handedness::LEFT, 0.25_rad, 0.75f, 0.1f, 100.0f);
	const auto expectedLH = Matrix4x4(
		{ 10.611053f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 7.958290f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, 1.001001f, 1.0f },
		{ 0.0f, 0.0f, -0.100100f, 0.0f }
		);

	BOOST_CHECK_EQUAL(perspectiveLH.matrix(), expectedLH);

	const auto perspectiveRH = Transform::perspectiveProjection(Handedness::RIGHT, 0.25_rad, 0.75f, 0.1f, 100.0f);
	const auto expectedRH = Matrix4x4(
		{ 10.611053f, 0.0f, 0.0f, 0.0f },
		{ 0.0f, 7.958290f, 0.0f, 0.0f },
		{ 0.0f, 0.0f, -1.001001f, -1.0f },
		{ 0.0f, 0.0f, -0.100100f, 0.0f }
		);

	BOOST_CHECK_EQUAL(perspectiveRH.matrix(), expectedRH);
}

BOOST_AUTO_TEST_CASE(PerspectiveProjectionWorks) {
	const auto perspective = Transform::perspectiveProjection(Handedness::LEFT, 90.0_deg, 0.5f, 1.0f, 100.0f);

	BOOST_CHECK_EQUAL(perspective.apply({ 0.0f, 0.0f, 1.0f, 1.0f }), Vec4(0.0f, 0.0f, 0.0f, 1.0f));
	BOOST_CHECK_EQUAL(perspective.apply({ 0.25f, 0.5f, 1.0f, 1.0f }), Vec4(0.5f, 0.5f, 0.0f, 1.0f));
	BOOST_CHECK_EQUAL(perspective.apply({ 0.5f, 1.0f, 1.0f, 1.0f }), Vec4(1.0f, 1.0f, 0.0f, 1.0f));

	BOOST_CHECK_EQUAL(perspective.apply({ 0.0f, 0.0f, 10.0f, 1.0f }), Vec4(0.0f, 0.0f, 0.909090f, 1.0f) * 10.0f);
	BOOST_CHECK_EQUAL(perspective.apply({ 2.5f, -5.0f, 10.0f, 1.0f }), Vec4(0.5f, -0.5f, 0.909090f, 1.0f) * 10.0f);
	BOOST_CHECK_EQUAL(perspective.apply({ 5.0f, -10.0f, 10.0f, 1.0f }), Vec4(1.0f, -1.0f, 0.909090f, 1.0f) * 10.0f);

	BOOST_CHECK_EQUAL(perspective.apply({ 0.0f, 0.0f, 90.0f, 1.0f }), Vec4(0.0f, 0.0f, 0.998877f, 1.0f) * 90.0f);
	BOOST_CHECK_EQUAL(perspective.apply({ 22.5f, -45.0f, 90.0f, 1.0f }), Vec4(0.5f, -0.5f, 0.998877f, 1.0f) * 90.0f);
	BOOST_CHECK_EQUAL(perspective.apply({ 45.0f, -90.0f, 90.0f, 1.0f }), Vec4(1.0f, -1.0f, 0.998877f, 1.0f) * 90.0f);

	BOOST_CHECK_EQUAL(perspective.apply({ 0.0f, 0.0f, 100.0f, 1.0f }), Vec4(0.0f, 0.0f, 1.0f, 1.0f) * 100.0f);
	BOOST_CHECK_EQUAL(perspective.apply({ -25.0f, -50.0f, 100.0f, 1.0f }), Vec4(-0.5f, -0.5f, 1.0f, 1.0f) * 100.0f);
	BOOST_CHECK_EQUAL(perspective.apply({ -50.0f, -100.0f, 100.0f, 1.0f }), Vec4(-1.0f, -1.0f, 1.0f, 1.0f) * 100.0f);
}

BOOST_AUTO_TEST_CASE(TranslationWorks) {
	auto translation = Transform::translation({ 1.0f, 2.0f, 3.0f });

	const auto start = Vec4(2.0f, 0.2f, 1.0f, 1.0f);
	const auto end = Vec4(3.0f, 2.2f, 4.0f, 1.0f);
	const auto translated = translation.apply(start);
	BOOST_CHECK_EQUAL(translated, end);
}

BOOST_AUTO_TEST_CASE(ScaleWorks) {
	auto scale = Transform::scale({ 1.0f, 2.0f, 3.0f });

	const auto start = Vec4(2.0f, 0.2f, 1.0f, 1.0f);
	const auto end = Vec4(2.0f, 0.4f, 3.0f, 1.0f);
	const auto scaled = scale.apply(start);
	BOOST_CHECK_EQUAL(scaled, end);
}

BOOST_AUTO_TEST_CASE(RotationWorks) {
	auto rotation = Transform::rotation(Vec3(2.0f, 1.0f, 3.0f), -23.0_deg);

	const auto start = Vec4(1.0f, 3.0f, 5.0f, 1.0f);
	const auto end = Vec4(1.565343f, 3.60607f, 4.421081f, 1.0f);
	const auto rotated = rotation.apply(start);
	BOOST_CHECK_EQUAL(rotated, end);
}

BOOST_AUTO_TEST_CASE(AppliesChainOfTransforms) {
	const auto start = Vec4(1.0f, 1.0f, 1.0f, 1.0f);
	const auto translation = Transform::translation({ 1.0f, 0.5f, -1.0f }); // -> <2.0f, 1.5f, 0.0f>
	const auto scale = Transform::scale({ 2.0f, 1.0f, 0.5f }); // -> <4.0f, 1.5f, 0.0f>
	const auto rotation = Transform::rotation({ 0.0f, 0.0f, 1.0f }, degrees(90.0f)); // -> <-1.5f, 4.0f, 0.0f>

	const auto end = translation.then(scale).then(rotation).apply(start);

	BOOST_CHECK_EQUAL(end, Vec4(-1.5f, 4.0f, 0.0f, 1.0f));
}

BOOST_AUTO_TEST_SUITE_END(/* PulpMathTransformTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpMathTestSuite */);
BOOST_AUTO_TEST_SUITE_END(/* PulpTestSuite */);

} // namespace anonymous
