#include "Math.hpp"

static constexpr f32 Deg2Rad = M_PI / 180.f;
static constexpr f32 Rad2Deg = 180.f / M_PI;

void Math::NormalizeAngles(QAngle& angle)
{
	angle.pitch = std::fmod(angle.pitch + 180.f, 360.f);
	if (angle.pitch < 0.f)
		angle.pitch += 360.f;
	angle.pitch -= 180.f;

	angle.yaw = std::fmod(angle.yaw + 180.f, 360.f);
	if (angle.yaw < 0.f)
		angle.yaw += 360.f;
	angle.yaw += 180.f;
}

f32 Math::GetFov(const QAngle& viewAngle, const QAngle& aimAngle)
{
	QAngle delta = aimAngle - viewAngle;
	Math::NormalizeAngles(delta);

	return std::sqrt(std::pow(delta.pitch, 2.0f) + std::pow(delta.yaw, 2.0f));
}

void Math::VectorAngles(const Vec3& forward, QAngle &angles)
{
	if (forward.y == 0.0f && forward.x == 0.0f)
	{
		angles.pitch = (forward.z > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
		angles.yaw = 0.0f;  //yaw left/right
	}
	else
	{
		angles.pitch = atan2(-forward.z, forward.Length2D()) * - Deg2Rad;
		angles.yaw = atan2(forward.y, forward.x) * Deg2Rad;

		if (angles.yaw > 90)
			angles.yaw -= 180;
		else if (angles.yaw < 90)
			angles.yaw += 180;
		else if (angles.yaw == 90)
			angles.yaw = 0;
	}

	angles.roll = 0.0f;
}

QAngle Math::CalcAngle(const Vec3& src, const Vec3& dst)
{
	QAngle angles;
	Vec3 delta = src - dst;

	Math::VectorAngles(delta, angles);

	delta.Normalize();

	return angles;
}
