#ifndef SDK_IAUDIODEVICE_HPP
#define SDK_IAUDIODEVICE_HPP

#include "Channel.hpp"

typedef u32 fixedint;

class CAudioMixer;
class CAudioSource;
class IAudioDevice : non_constructible
{
public:
	// This initializes the sound hardware.  true on success, false on failure
	virtual bool Init() = 0;

	// This releases all sound hardware
	virtual void Shutdown() = 0;

	// device parameters
	virtual const char* DeviceName() const = 0;
	virtual i32 DeviceChannels() const = 0; // 1 = mono, 2 = stereo
	virtual i32 DeviceSampleBits() const = 0; // bits per sample (8 or 16)
	virtual i32 DeviceSampleBytes() const = 0; // above / 8
	virtual i32 DeviceSampleRate() const = 0; // Actual DMA speed
	virtual i32 DeviceSampleCount() const = 0; // Total samples in buffer

	// Called each time a new paint buffer is mixed (may be multiple times per frame)
	virtual void MixBegin() = 0;

	// Main mixing routines
	virtual void Mix8Mono(Channel* channel, char* data, i32 outputOffset, i32 inputOffset, fixedint rateScaleFix, i32 outCount, i32 timecompress, bool forward = true) = 0;
	virtual void Mix8Stereo(Channel* channel, char* data, i32 outputOffset, i32 inputOffset, fixedint rateScaleFix, i32 outCount, i32 timecompress, bool forward = true) = 0;
	virtual void Mix16Mono(Channel* channel, short* data, i32 outputOffset, i32 inputOffset, fixedint rateScaleFix, i32 outCount, i32 timecompress, bool forward = true) = 0;
	virtual void Mix16Stereo(Channel* channel, short* data, i32 outputOffset, i32 inputOffset, fixedint rateScaleFix, i32 outCount, i32 timecompress, bool forward = true) = 0;

	// Size of the paint buffer in samples
	virtual i32 PaintBufferSampleCount() const = 0;

	// Adds a mixer to be mixed
	virtual void AddSource(CAudioMixer* pSource) = 0;

	// Stops all sounds
	virtual void StopSounds() = 0;

	// Updates sound mixing
	virtual void Update(f32 time) = 0;

	// Resets the device
	virtual void Flush() = 0;

	virtual i32 FindSourceIndex(CAudioMixer* source) = 0;
	virtual CAudioMixer* GetMixerForSource(CAudioSource* source) = 0;
	virtual void FreeChannel(i32 channelIndex) = 0;
};

#endif // SDK_IAUDIODEVICE_HPP
