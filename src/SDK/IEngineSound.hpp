#ifndef SDK_IENGINESOUND_HPP
#define SDK_IENGINESOUND_HPP

#include "Defs.hpp"
#include "CUtl/Vector.hpp"
class IRecipientFilter;

MAKE_CENUM_Q(SoundFlags, u32,
	NOFLAGS, 0, // to keep the compiler happy
	CHANGE_VOL, (1 << 0), // change sound vol
	CHANGE_PITCH, (1 << 1), // change sound pitch
	STOP, (1 << 2), // stop the sound
	SPAWNING, (1 << 3), // we're spawning, used in some cases for ambients
	// not sent over net, only a param between dll and server.
	DELAY, (1 << 4), // sound has an initial delay
	STOP_LOOPING, (1 << 5), // stop all looping sounds on the entity.
	SPEAKER, (1 << 6), // being played again by a microphone through a speaker

	SHOULDPAUSE, (1 << 7), // this sound should be paused if the game is paused
	IGNORE_PHONEMES, (1 << 8),
	IGNORE_NAME, (1 << 9), // used to change all sounds emitted by an entity, regardless of scriptname
	IS_SCRIPTHANDLE, (1 << 10), // server has passed the actual SoundEntry instead of wave filename

	UPDATE_DELAY_FOR_CHOREO, (1 << 11), // True if we have to update snd_delay_for_choreo with the IO latency.
	GENERATE_GUID, (1 << 12), // True if we generate the GUID when we send the sound.

	OVERRIDE_PITCH, (1 << 13), // The pitch given in code overrides what is present in the sound entry.
);

MAKE_CENUM_Q(SoundLevel, u32,
	NONE, 0,

	L20DB, 20, // rustling leaves
	L25DB, 25, // whispering
	L30DB, 30, // library
	L35DB, 35,
	L40DB, 40,
	L45DB, 45, // refrigerator

	L50DB, 50, // 3.9// average home
	L55DB, 55, // 3.0

	IDLE, 60, // 2.0
	L60DB, 60, // 2.0// normal conversation, clothes dryer

	L65DB, 65, // 1.5// washing machine, dishwasher
	STATIC, 66, // 1.25
	
	L70DB, 70, // 1.0// car, vacuum cleaner, mixer, electric sewing machine

	NORM, 75,
	L75DB, 75, // 0.8// busy traffic

	L80DB, 80, // 0.7// mini-bike, alarm clock, noisy restaurant, office tabulator, outboard motor, passing snowmobile
	TALKING, 80, // 0.7
	L85DB, 85, // 0.6// average factory, electric shaver
	L90DB, 90, // 0.5// screaming child, passing motorcycle, convertible ride on frw
	L95DB, 95,
	L100DB, 100, // 0.4// subway train, diesel truck, woodworking shop, pneumatic drill, boiler shop, jackhammer
	L105DB, 105, // helicopter, power mower
	L110DB, 110, // snowmobile drvrs seat, inboard motorboat, sandblasting
	L120DB, 120, // auto horn, propeller aircraft
	L130DB, 130, // air raid siren

	GUNFIRE, 140, // 0.27// THRESHOLD OF PAIN, gunshot, jet engine
	L140DB, 140, // 0.2

	L150DB, 150, // 0.2

	L180DB, 180, // rocket launching
	// NOTE: Valid SoundLevel values are 0-255.
	//       256-511 are reserved for sounds using goldsrc compatibility attenuation.
);
struct SndInfo;

#define PITCH_NORM 100// non-pitch shifted
#define PITCH_LOW 95// other values are possible - 0-255, where 255 is very high
#define PITCH_HIGH 120

#define SOUNDEMITTER_INVALID_HASH (HSOUNDSCRIPTHASH)-1
typedef u32 HSOUNDSCRIPTHASH;

class IEngineSound : public non_constructible
{
	// Precache a particular sample
	virtual bool PrecacheSound(const char* sample, bool preload = false, bool isUISound = false) = 0;
	virtual bool IsSoundPrecached(const char* sample) = 0;
	virtual void PrefetchSound(const char* sample) = 0;
	virtual bool IsLoopingSound(const char* sample) = 0;

	// Just loads the file header and checks for duration (not hooked up for .mp3's yet)
	// Is accessible to server and client though
	virtual f32 GetSoundDuration(const char* sample) = 0;

	// Pitch of 100 is no pitch shift.  Pitch > 100 up to 255 is a higher pitch, pitch < 100
	// down to 1 is a lower pitch.   150 to 70 is the realistic range.
	// EmitSound with pitch != 100 should be used sparingly, as it's not quite as
	// fast (the pitchshift mixer is not native coded).

	// NOTE: setting iEntIndex to -1 will cause the sound to be emitted from the local
	// player (client-side only)
	// Will return the sound guid. If negative, the guid is unknown (call may be successful or not). 0 if the sound was not emitted. Positive if the guid is valid.
	virtual i32 EmitSound(IRecipientFilter& filter, i32 entIndex, i32 channel, const char* soundEntry, HSOUNDSCRIPTHASH soundEntryHash, const char* sample, f32 volume, f32 attenuation, i32 seed, i32 flags = 0, i32 pitch = PITCH_NORM, const Vec3* origin = NULL, const Vec3* direction = NULL, CUtlVector<Vec3>* origins = NULL, bool updatePositions = true, f32 soundtime = 0.0f, i32 speakerentity = -1) = 0;

	virtual i32 EmitSound(IRecipientFilter& filter, i32 entIndex, i32 channel, const char* soundEntry, HSOUNDSCRIPTHASH soundEntryHash, const char* sample, f32 flVolume, SoundLevel soundlevel, i32 seed, i32 flags = 0, i32 pitch = PITCH_NORM, const Vec3* origin = NULL, const Vec3* direction = NULL, CUtlVector<Vec3>* vOrigins = NULL, bool updatePositions = true, f32 soundtime = 0.0f, i32 speakerentity = -1) = 0;

	virtual void EmitSentenceByIndex(IRecipientFilter& filter, i32 entIndex, i32 channel, i32 sentenceIndex, f32 volume, SoundLevel soundlevel, i32 seed, i32 flags = 0, i32 pitch = PITCH_NORM, const Vec3* pOrigin = NULL, const Vec3* direction = NULL, CUtlVector<Vec3>* origins = NULL, bool updatePositions = true, f32 soundtime = 0.0f, i32 speakerentity = -1) = 0;

	virtual void StopSound(i32 entIndex, i32 channel, const char* sample, HSOUNDSCRIPTHASH soundEntryHash = SOUNDEMITTER_INVALID_HASH) = 0;

	// stop all active sounds (client only)
	virtual void StopAllSounds(bool clearBuffers) = 0;

	// Set the room type for a player (client only)
	virtual void SetRoomType(IRecipientFilter& filter, i32 roomType) = 0;

	// Set the dsp preset for a player (client only)
	virtual void SetPlayerDSP(IRecipientFilter& filter, i32 dspType, bool fastReset) = 0;

	// emit an "ambient" sound that isn't spatialized
	// only available on the client, assert on server
	virtual i32 EmitAmbientSound(const char* sample, f32 volume, i32 pitch = PITCH_NORM, i32 flags = 0, f32 soundtime = 0.0f) = 0;


	//	virtual EntChannel_t	CreateEntChannel() = 0;

	virtual f32 GetDistGainFromSoundLevel(SoundLevel soundlevel, f32 dist) = 0;

	// Client .dll only functions
	virtual i32 GetGuidForLastSoundEmitted() = 0;
	virtual bool IsSoundStillPlaying(i32 guid) = 0;
	virtual void StopSoundByGuid(i32 guid, bool bForceSync = false) = 0;
	// Set's master volume (0.0->1.0)
	virtual void SetVolumeByGuid(i32 guid, f32 fvol) = 0;

	// Retrieves list of all active sounds
	virtual void GetActiveSounds(CUtlVector<SndInfo>& sndlist) = 0;

	virtual void PrecacheSentenceGroup(const char* pGroupName) = 0;
	virtual void NotifyBeginMoviePlayback() = 0;
	virtual void NotifyEndMoviePlayback() = 0;
	virtual bool IsMoviePlaying() = 0;

	virtual bool GetSoundChannelVolume(const char* sound, f32& volumeLeft, f32& volumeRight) = 0;

	virtual f32 GetElapsedTimeByGuid(i32 guid) = 0;

	// engine capable of playing sounds?
	virtual bool GetPreventSound() = 0;

	virtual void SetReplaySoundFade(f32 flReplayVolume) = 0;
	virtual f32 GetReplaySoundFade() const = 0;
#if defined(_GAMECONSOLE)
	virtual void UnloadSound(const char* pSample) = 0;
#endif
};

#endif // SDK_IENGINESOUND_HPP
