#ifndef SDK_CAUDIOMIXER_HPP
#define SDK_CAUDIOMIXER_HPP

#include "IAudioDevice.hpp"

class CAudioSource;
class CAudioMixer
{
public:
	virtual ~CAudioMixer() = 0;

	// UNDONE: time compress
	virtual bool MixDataToDevice(IAudioDevice* device, Channel* channel, i32 startSample, i32 sampleCount, i32 outputRate, bool forward = true) = 0;
	virtual void IncrementSamples(Channel* channel, i32 startSample, i32 sampleCount, i32 outputRate, bool forward = true) = 0;
	virtual bool SkipSamples(IAudioDevice* device, Channel* channel, i32 startSample, i32 sampleCount, i32 outputRate, bool forward = true) = 0;

	virtual CAudioSource* GetSource() = 0;

	virtual i32 GetSamplePosition() = 0;
	virtual i32 GetScubPosition() = 0;

	virtual bool SetSamplePosition(i32 position, bool scrubbing = false) = 0;
	virtual void SetLoopPosition(i32 position) = 0;
	virtual i32 GetStartPosition() = 0;

	virtual bool GetActive() = 0;
	virtual void SetActive(bool active) = 0;

	virtual void SetModelIndex(i32 index) = 0;
	virtual i32 GetModelIndex() const = 0;

	virtual void SetDirection(bool forward) = 0;
	virtual bool GetDirection() const = 0;

	virtual void SetAutoDelete(bool autodelete) = 0;
	virtual bool GetAutoDelete() const = 0;

	virtual void SetVolume(f32 volume) = 0;
	virtual Channel* GetChannel() = 0;
};

#endif // SDK_CAUDIOMIXER_HPP
