#ifndef SDK_CCSGOANIMSTATE_HPP
#define SDK_CCSGOANIMSTATE_HPP


class CCSGOAnimState
{
public:
	char pad_0008[128]; //0x0000
	f32 lastAnimUpdateTime; //0x0080
	i32 lastAnimUpdateFrame; //0x0084
	f32 eyePitch; //0x0088
	f32 yaw; //0x008C
	f32 pitch; //0x0090
	f32 goalFeetYaw; //0x0094
	f32 currentFeetYaw; //0x0098
	f32 absMovementDirection; //0x009C  Movement direction on a compass, North = 0, East = 90, South = 180...
	f32 lastAbsMovementDirection; //0x00A0 ^^^^^ for last tick
	f32 leanAmount; //0x00A4
	char pad_00A8[4]; //0x00A8
	f32 feetCycle; //0x00AC Progress from 0.0-1.0 of the legs moving animation.
	f32 feetYawRate; //0x00B0 How fast to play feetCycle ( walking = slower )
	char pad_00B4[4]; //0x00B4
	f32 duckProgress; //0x00B8 Progress 0.0-1.0, 0.0 = standing, 1.0 = fully crouched. Also used for jumping. Counts backwards from 1.0 for jumps
	f32 landingAnimationTimeLeftUntilDone; //0x00BC Adds some time when landing animation starts.
	char pad_00C0[4]; //0x00C0
	Vec3 origin; //0x00C4
	Vec3 lastOrigin; //0x00D0
	f32 velocityX; //0x00DC Speeds on axis
	f32 velocityY; //0x00E0 ^^^^^^^^^^
	char pad_00E4[4]; //0x00E4
	f32 currentDirectionX; //0x00E8 -1.0 to 1.0. West is 1.0, East is -1.0. North and South are both 0
	f32 currentDirectionY; //0x00EC -1.0 to 1.0. North is -1.0, South is 1.0. East and West are both 0
	char pad_00F0[4]; //0x00F0
	f32 lastKnownDirectionX; //0x00F4 Same as its counterpart above, but this is saved until the next update
	f32 lastKnownDirectionY; //0x00F8 ^^^^^^^^^^^^^^^^^^^^^^^
	char pad_00FC[4]; //0x00FC
	f32 horizontalVelocity; //0x0100 Affected by slopes. ( got 240 with knife on a hill )
	f32 verticalVelocity; //0x0104. Goes up when jumping, negative when falling
	f32 speed; //0x0108 0.0 -> 1.0, how fast you are going; 1.0 = full speed.
	f32 feetShuffleSpeed; //0x010C // 0.0-2.0 (approx)
	f32 feetShuffleSpeed2; //0x0110 // 0.0-3.0 (approx)
	f32 timeSinceStartedMoving; //0x0114 Jumping and Crouching do not affect these
	f32 timeSinceStoppedMoving; //0x0118 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	bool onGround; //0x011C True if on ground (do I need to type this?)
	bool inJumpRecoveryAnim; //0x011D True if you're doing one of the retarded knee buckling animations
	char pad_011E[10]; //0x011E
	f32 heightBeforeJump; //0x0128 Updated right when you jump
	char pad_012C[4]; //0x012C
	f32 runningAccelProgress; //0x0130 0 - 1.0, Only affected by Running, reaches 1.0 when you are at full speed (ex: 250 w/ knife )
	char pad_0134[68]; //0x0134
	char unkown[572];// Overall Size should be 0x3B0(+4), padding the end here.
}; //Size: 0x0178

#endif // SDK_CCSGOANIMSTATE_HPP
