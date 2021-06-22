#ifndef SDK_IVDEBUGOVERLAY_HPP
#define SDK_IVDEBUGOVERLAY_HPP

#include "Defs.hpp"
struct OverlayText; // OverlayText_t

class IVDebugOverlay
{
public:
	virtual void AddEntityTextOverlay(i32 ent_index, i32 line_offset, f32 duration, i32 r, i32 g, i32 b, i32 a, const char *format, ...) = 0;
	virtual void AddBoxOverlay(const Vec3& origin, const Vec3& mins, const Vec3& max, QAngle const& orientation, i32 r, i32 g, i32 b, i32 a, f32 duration) = 0;
	virtual void AddSphereOverlay(const Vec3& vOrigin, f32 flRadius, i32 nTheta, i32 nPhi, i32 r, i32 g, i32 b, i32 a, f32 flDuration) = 0;
	virtual void AddTriangleOverlay(const Vec3& p1, const Vec3& p2, const Vec3& p3, i32 r, i32 g, i32 b, i32 a, bool noDepthTest, f32 duration) = 0;
	virtual void AddLineOverlay(const Vec3& origin, const Vec3& dest, i32 r, i32 g, i32 b,bool noDepthTest, f32 duration) = 0;
	virtual void AddTextOverlay(const Vec3& origin, f32 duration, const char *format, ...) = 0;
	virtual void AddTextOverlay(const Vec3& origin, i32 line_offset, f32 duration, const char *format, ...) = 0;
	virtual void AddScreenTextOverlay(f32 flXPos, f32 flYPos,f32 flDuration, i32 r, i32 g, i32 b, i32 a, const char *text) = 0;
	virtual void AddSweptBoxOverlay(const Vec3& start, const Vec3& end, const Vec3& mins, const Vec3& max, const QAngle & angles, i32 r, i32 g, i32 b, i32 a, f32 flDuration) = 0;
	virtual void AddGridOverlay(const Vec3& origin) = 0;
	virtual void AddCoordFrameOverlay(const Mat3x4& frame, f32 flScale, i32 vColorTable[3][3] = NULL) = 0;

	virtual bool ScreenPosition(const Vec3& point, Vec3& screen) = 0;
	virtual i32 ScreenPosition2(f32 flXPos, f32 flYPos, Vec3& screen) = 0;

	virtual OverlayText *GetFirst( void ) = 0;
	virtual OverlayText *GetNext( OverlayText *current ) = 0;
	virtual void ClearDeadOverlays( void ) = 0;
	virtual void ClearAllOverlays() = 0;

	virtual void AddTextOverlayRGB(const Vec3& origin, i32 line_offset, f32 duration, f32 r, f32 g, f32 b, f32 alpha, const char *format, ...) = 0;
	virtual void AddTextOverlayRGB(const Vec3& origin, i32 line_offset, f32 duration, i32 r, i32 g, i32 b, i32 a, const char *format, ...) = 0;

	virtual void AddLineOverlayAlpha(const Vec3& origin, const Vec3& dest, i32 r, i32 g, i32 b, i32 a, bool noDepthTest, f32 duration) = 0;
	virtual void AddBoxOverlay2( const Vec3& origin, const Vec3& mins, const Vec3& max, QAngle const& orientation, const Color& faceColor, const Color& edgeColor, f32 duration ) = 0;

	virtual void AddLineOverlay(const Vec3& origin, const Vec3& dest, i32 r, i32 g, i32 b, i32 a, f32 thickness, f32 duration) = 0;

	virtual void PurgeTextOverlays() = 0;

	virtual void AddCapsuleOverlay( const Vec3 &vStart, const Vec3 &vEnd, const f32 &flRadius, i32 r, i32 g, i32 b, i32 a, f32 flDuration ) = 0;
};

#endif // SDK_IVDEBUGOVERLAY_HPP
