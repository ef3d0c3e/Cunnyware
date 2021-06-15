#ifndef SDK_INETCHANNELINFO_HPP
#define SDK_INETCHANNELINFO_HPP

#include "../Util/Defs.hpp"

MAKE_CENUM_Q(NetChanFlow, u32,
	OUTGOING, 0,
	INCOMING, 1);

class INetChannelInfo
{
public:
	MAKE_CENUMV_Q(SignalType /*TODO: Better name*/, u32,
		GENERIC, 0, // must be first and is default group
		LOCALPLAYER, 1, // bytes for local player entity update
		OTHERPLAYERS, 2, // bytes for other players update
		ENTITIES, 3, // all other entity bytes
		SOUNDS, 4, // game sounds
		EVENTS, 5, // event messages
		TEMPENTS, 6, // temp entities
		USERMESSAGES, 7, // user messages
		ENTMESSAGES, 8, // entity messages
		VOICE, 9, // voice data
		STRINGTABLE, 10, // a stringtable update
		MOVE, 11, // client move cmds
		STRINGCMD, 12, // string command
		SIGNON, 13, // various signondata
		TOTAL, 14, // must be last and is not a real group
	);

	virtual const char* GetName() const = 0; // get channel name
	virtual const char* GetAddress() const = 0; // get channel IP address as string
	virtual f32 GetTime() const = 0; // current net time
	virtual f32 GetTimeConnected() const = 0; // get connection time in seconds
	virtual i32 GetBufferSize() const = 0; // netchannel packet history size
	virtual i32 GetDataRate() const = 0; // send data rate in byte/sec

	virtual bool IsLoopback() const = 0; // true if loopback channel
	virtual bool IsTimingOut() const = 0; // true if timing out
	virtual bool IsPlayback() const = 0; // true if demo playback

	virtual f32 GetLatency(i32 flow) const = 0; // current latency (RTT), more accurate but jittering
	virtual f32 GetAvgLatency(i32 flow) const = 0; // average packet latency in seconds
	virtual f32 GetAvgLoss(i32 flow) const = 0; // avg packet loss[0..1]
	virtual f32 GetAvgChoke(i32 flow) const = 0; // avg packet choke[0..1]
	virtual f32 GetAvgData(i32 flow) const = 0; // data flow in bytes/sec
	virtual f32 GetAvgPackets(i32 flow) const = 0; // avg packets/sec
	virtual i32 GetTotalData(i32 flow) const = 0; // total flow in/out in bytes
	virtual i32 GetTotalPackets(i32 flow) const = 0;
	virtual i32 GetSequenceNr(i32 flow) const = 0; // last send seq number
	virtual bool IsValidPacket(i32 flow, i32 frame_number) const = 0; // true if packet was not lost/dropped/chocked/flushed
	virtual f32 GetPacketTime(i32 flow, i32 frame_number) const = 0; // time when packet was send
	virtual i32 GetPacketBytes(i32 flow, i32 frame_number, i32 group) const = 0; // group size of this packet
	virtual bool GetStreamProgress(i32 flow, i32* received, i32* total) const = 0; // TCP progress if transmitting
	virtual f32 GetTimeSinceLastReceived() const = 0; // get time since last recieved packet in seconds
	virtual f32 GetCommandInterpolationAmount(i32 flow, i32 frame_number) const = 0;
	virtual void GetPacketResponseLatency(i32 flow, i32 frame_number, i32* pnLatencyMsecs, i32* pnChoke) const = 0;
	virtual void GetRemoteFramerate(f32* pflFrameTime, f32* pflFrameTimeStdDeviation) const = 0;

	virtual f32 GetTimeoutSeconds() const = 0;
};

#endif // SDK_INETCHANNELINFO_HPP
