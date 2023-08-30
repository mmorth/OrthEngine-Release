#include <gtest/gtest.h>

#include <bullet/btBulletDynamicsCommon.h>

#include "BulletTypeConversions.hpp"

// ------------------------------------------------------------------------
class BulletTypeConversionsTest : public testing::Test
{};

// ------------------------------------------------------------------------
TEST_F(BulletTypeConversionsTest, PhysicsUtilsConvertMathUtilVec3ToBtVector3)
{
	MathUtils::Vec3 mathVec3(1.0f, 2.0f, 3.0f);

	// FUT
	btVector3 btVec3 = BtConversions::mathToBtVec3(mathVec3);

	EXPECT_EQ(btVec3.x(), mathVec3.x);
	EXPECT_EQ(btVec3.y(), mathVec3.y);
	EXPECT_EQ(btVec3.z(), mathVec3.z);
}

// ------------------------------------------------------------------------
TEST_F(BulletTypeConversionsTest, PhysicsUtilsConvertMathUtilVec4ToBtVector4)
{
	MathUtils::Vec4 mathVec4(1.0f, 2.0f, 3.0f, 4.0f);

	// FUT
	btQuaternion btVec4 = BtConversions::mathToBtVec4(mathVec4);

	EXPECT_EQ(btVec4.x(), mathVec4.x);
	EXPECT_EQ(btVec4.y(), mathVec4.y);
	EXPECT_EQ(btVec4.z(), mathVec4.z);
	EXPECT_EQ(btVec4.w(), mathVec4.w);
}

// ------------------------------------------------------------------------
TEST_F(BulletTypeConversionsTest, PhysicsUtilsConvertBtVector3ToMathUtilsVector3)
{
	btVector3 btVec3(1.0f, 2.0f, 3.0f);

	// FUT
	MathUtils::Vec3 mathVec3 = BtConversions::btToMathVec3(btVec3);

	EXPECT_EQ(mathVec3.x, btVec3.x());
	EXPECT_EQ(mathVec3.y, btVec3.y());
	EXPECT_EQ(mathVec3.z, btVec3.z());
}

// ------------------------------------------------------------------------
TEST_F(BulletTypeConversionsTest, PhysicsUtilsConvertBtVector4ToMathUtilsVector4)
{
	btQuaternion btVec4(1.0f, 2.0f, 3.0f, 4.0f);

	// FUT
	MathUtils::Vec4 mathVec4 = BtConversions::btToMathVec4(btVec4);

	EXPECT_EQ(mathVec4.x, btVec4.x());
	EXPECT_EQ(mathVec4.y, btVec4.y());
	EXPECT_EQ(mathVec4.z, btVec4.z());
	EXPECT_EQ(mathVec4.w, btVec4.w());
}
