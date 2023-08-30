#ifndef BULLETCONVERSIONS_HPP
#define BULLETCONVERSIONS_HPP

#include <bullet/btBulletDynamicsCommon.h>

#include "MathUtils.hpp"

namespace BtConversions
{
	// Vec3 conversions
	MathUtils::Vec3 btToMathVec3(const btVector3& btVec3);
	btVector3 mathToBtVec3(const MathUtils::Vec3& mathVec3);

	// Vec4 conversions
	MathUtils::Vec4 btToMathVec4(const btQuaternion& btVec4);
	btQuaternion mathToBtVec4(const MathUtils::Vec4& mathVec4);
}

#endif // BULLETCONVERSIONS_HPP
