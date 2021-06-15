#ifndef SDK_CAUDIOSOURCE_HPP
#define SDK_CAUDIOSOURCE_HPP

#include "CAudioMixer.hpp"
#include "CSentence.hpp"

class CAudioSource : non_constructible
{
public:
	virtual ~CAudioSource() = 0;

	// Create an instance (mixer) of this audio source
	virtual CAudioMixer* CreateMixer() = 0;
	virtual i32 GetOutputData(void** data, i32 samplePosition, i32 sampleCount, bool forward = true) = 0;
	virtual i32 SampleRate() = 0;
	virtual i32 SampleSize() = 0;
	virtual i32 SampleCount() = 0;
	virtual f32 TrueSampleSize() = 0;
	virtual bool IsLooped() = 0;
	virtual bool IsStreaming() = 0;
	virtual f32 GetRunningLength() = 0;
	virtual i32 GetNumChannels() = 0;

	virtual CSentence* GetSentence() = 0;
};

#endif // SDK_CAUDIOSOURCE_HPP
