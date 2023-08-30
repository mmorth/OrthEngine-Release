#include "BulletTypeConversions.hpp"

namespace BtConversions
{
	// ------------------------------------------------------------------------
	MathUtils::Vec3 btToMathVec3(const btVector3& btVec3) { return MathUtils::Vec3(btVec3.x(), btVec3.y(), btVec3.z()); }

	// ------------------------------------------------------------------------
	btVector3 mathToBtVec3(const MathUtils::Vec3& mathVec3) { return btVector3(mathVec3.x, mathVec3.y, mathVec3.z); }

	// ------------------------------------------------------------------------
	MathUtils::Vec4 btToMathVec4(const btQuaternion& btVec4) { return MathUtils::Vec4(btVec4.x(), btVec4.y(), btVec4.z(), btVec4.w()); }

	// ------------------------------------------------------------------------
	btQuaternion mathToBtVec4(const MathUtils::Vec4& mathVec4) { return btQuaternion(mathVec4.x, mathVec4.y, mathVec4.z, mathVec4.w); }
}