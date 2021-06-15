#ifndef SDK_CSENTENCE_HPP
#define SDK_CSENTENCE_HPP

#include "CBasePhonemeTag.hpp"
#include "CEmphasisSample.hpp"
#include "CUtl/Vector.hpp"

class CSentence : non_constructible
{
public:
	CUtlVector<CBasePhonemeTag*> runTimePhonemes;

	// Phoneme emphasis data
	CUtlVector<CEmphasisSample> emphasisSamples;

	bool isValid : 8;
	bool storeCheckSum : 8;
	bool shouldVoiceDuck : 8;
	bool isCached : 8;
};

#endif // SDK_CSENTENCE_HPP
