#include "Math.hpp"

static constexpr f32 Deg2Rad = M_PI / 180.f;
static constexpr f32 Rad2Deg = 180.f / M_PI;

void Math::NormalizeAngles(QAngle& angle) noexcept
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

f32 Math::GetFov(const QAngle& viewAngle, const QAngle& aimAngle) noexcept
{
	QAngle delta = aimAngle - viewAngle;
	Math::NormalizeAngles(delta);

	return std::sqrt(std::pow(delta.pitch, 2.0f) + std::pow(delta.yaw, 2.0f));
}

void Math::VectorAngles(const Vec3& forward, QAngle& angles) noexcept
{
	if (forward.y == 0.0f && forward.x == 0.0f)
	{
		angles.pitch = (forward.z > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
		angles.yaw = 0.0f; //yaw left/right
	}
	else
	{
		angles.pitch = atan2(-forward.z, forward.Length2D()) * -Deg2Rad;
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

QAngle Math::CalcAngle(const Vec3& src, const Vec3& dst) noexcept
{
	QAngle angles;
	Vec3 delta = src - dst;

	Math::VectorAngles(delta, angles);

	delta /= delta.Length();

	return angles;
}

void Math::AngleVectors(const QAngle& angles, Vec3* forward, Vec3* right, Vec3* up) noexcept
{
	const f32 sr = sin(angles.roll), sp = sin(angles.pitch), sy = sin(angles.yaw), cr = cos(angles.roll), cp = cos(angles.pitch), cy = cos(angles.yaw);

	if (forward)
	{
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if (right)
	{
		right->x = (-1.f * sr * sp * cy + -1.f * cr * -sy);
		right->y = (-1.f * sr * sp * sy + -1.f * cr * cy);
		right->z = -1.f * sr * cp;
	}

	if (up)
	{
		up->x = (cr * sp * cy + -sr * -sy);
		up->y = (cr * sp * sy + -sr * cy);
		up->z = cr * cp;
	}
}

Vec3 Math::AngleForward(const QAngle& angles) noexcept
{
	const f32 sp = sin(angles.pitch), sy = sin(angles.yaw), cp = cos(angles.pitch), cy = cos(angles.yaw);

	return Vec3{cp * cy, cp * sy, -sp};
}

std::pair<Math::IntersectionType, Vec2> Math::Intersection(const Rect2& s1, const Rect2& s2, f32 eps) noexcept
{
	const auto isZero = [eps](f32 x) { return x < eps && x > -eps; };
	const auto cross = [](Vec2 u, Vec2 v) { return u.x * v.y - u.y * v.x; };

	const Vec2 r = s1.y - s1.x;
	const Vec2 s = s2.y - s2.x;

	const f32 rxs = cross(r, s);
	if (isZero(rxs)) [[unlikely]]
		return std::make_pair(IntersectionType::COLL, Vec2{});

	const f32 t = cross(s2.x - s1.x, s) / rxs;
	const f32 u = cross(s2.x - s1.x, r) / rxs;

	if (-eps <= t &&
		t <= 1.f+eps &&
		-eps <= u &&
		u <= 1.f+eps)
		return std::make_pair(IntersectionType::POINT, s1.x + r*t);

	return std::make_pair(IntersectionType::NONE, Vec2{});
}
