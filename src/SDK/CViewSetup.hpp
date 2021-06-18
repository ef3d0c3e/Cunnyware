#ifndef SDK_CVIEWSETUP_HPP
#define SDK_CVIEWSETUP_HPP

#include "../Util/Defs.hpp"

MAKE_CENUM_Q(MotionBlurMode, u32,
	DISABLED, 1,
	GAME, 2,
	SFM, 3);

struct CViewSetup : public non_constructible
{
	i32 x;
	i32 oldX;
	i32 y;
	i32 oldY;
	i32 width;
	i32 oldWidth;
	i32 height;
	i32 oldHeight;
	char pad_0020[152];
	f32 fov;
	f32 fovViewModel;
	Vec3 origin;
	Vec3 angles;
	f32 zNear;
	f32 zFar;
	f32 zNearViewmodel;
	f32 zFarViewmodel;
	f32 aspectRatio;
	f32 nearBlurDepth;
	f32 nearFocusDepth;
	f32 farFocusDepth;
	f32 farBlurDepth;
	f32 nearBlurRadius;
	f32 farBlurRadius;
	f32 doFQuality;
	i32 motionBlurMode;

	f32 shutterTime;
	Vec3 shutterOpenPosition;
	QAngle shutterOpenAngles;
	Vec3 shutterClosePosition;
	QAngle shutterCloseAngles;

	f32 offCenterTop;
	f32 offCenterBottom;
	f32 offCenterLeft;
	f32 offCenterRight;

	bool offCenter : 1;
	bool renderToSubrectOfLargerScreen : 1;
	bool doBloomAndToneMapping : 1;
	bool doDepthOfField : 1;
	bool HDRTarget : 1;
	bool drawWorldNormal : 1;
	bool cullFrontFaces : 1;
	bool cacheFullSceneState : 1;
	bool renderFlashlightDepthTranslucents : 1;
};

#endif // SDK_CVIEWSETUP_HPP
