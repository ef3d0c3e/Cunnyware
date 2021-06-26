#ifndef SDK_CSTUDIOHDR_HPP
#define SDK_CSTUDIOHDR_HPP

#include "Defs.hpp"

struct MStudioBone
{
	i32 nameOffset;

	inline char const* GetName() const
	{
		return ((char*) this) + nameOffset;
	}
	
	i32 parent; ///< Parent bone
	std::array<i32, 6> boneController; // Bone controller index, -1 == none

	Vec3 pos;
	Quaternion quat;
	RadianEuler Rot;
	Vec3 posScale;
	Vec3 rotScale;
	
	Mat3x4 poseToBone;
	Quaternion alignment;
	i32 flags;
	i32 procType;
	i32 procIdx; ///< Procedural rule
	mutable i32 physicsBone; ///< Index int o physically simulated bone

	inline void* Procedure() const
	{
		if (procIdx == 0)
			return nullptr;
		else
			return (void*) (((u8*) this) + procIdx);
	};

	i32 surfacePropIdx; ///< Index into string table for property name

	inline char* const SurfaceProp() const
	{
		return ((char*)this) + surfacePropIdx;
	}

	inline i32 GetSurfaceProp() const
	{
		return surfacePropLookup;
	}

	i32 contents; ///< See BSPFlags.h for the contents flags
	i32 surfacePropLookup; ///< this index must be cached by the loader, not saved in the file
	std::array<i32, 7> unused;
};

struct MStudioBox
{
	i32 bone;
	i32 group;
	Vec3 mins;
	Vec3 maxs;
	i32 hitboxNameIdx;
	std::array<int, 3> pad;
	f32 radius;
	std::array<int, 4> pad2;

	char* HitboxName()
	{
		if (hitboxNameIdx == 0)
			return nullptr;

		return ((char*)this) + hitboxNameIdx;
	}
};

struct MStudioHitboxSet
{
	i32 nameIdx;

	inline char* const Name() const
	{
		return ((char*)this) + nameIdx;
	}

	i32 numHitboxes;
	i32 hitboxIdx;

	inline MStudioBox* Hitbox(i32 i) const
	{
		return (MStudioBox*) (((u8*)this) + hitboxIdx) + i;
	}
};

struct StudioHdr
{
	i32 id;
	i32 version;
	i32 checksum; ///< This has to be the same in the phy and vtx files to load!
	std::array<char, 64> name;
	i32 length;

	Vec3 eyePosition; ///< Ideal eye position
	Vec3 illumPosition; ///< Illumination center
	Vec3 hullMin; ///< Ideal movement hull size
	Vec3 hullMax;
	Vec3 viewBBMin; ///< Clipping bounding box
	Vec3 viewBBMax;

	i32 flags;
	i32 numBones; ///< Bones
	i32 boneIdx;
	inline MStudioBone* Bone(i32 i) const
	{
		assert(i >= 0 && i < numBones);
		return (MStudioBone*)(((u8* ) this) + boneIdx) + i;
	};

	i32 RemapSeqBone(i32 sequence, i32 localBone) const; ///< Maps local sequence bone to global bone
	i32 RemapAnimBone(i32 anim, i32 localBone) const; ///< Maps local animations bone to global bone
	i32 numBoneControllers; ///< Bone controllers
	i32 boneControllerIdx;
	i32 numHitboxSets;
	i32 hitboxSetIdx;

	// Look up hitbox set by index
	MStudioHitboxSet* HitboxSet(i32 i) const
	{
		(i >= 0 && i < numHitboxSets);
		return (MStudioHitboxSet*)(((u8*) this ) + hitboxSetIdx ) + i;
	};

	// Calls through to hitbox to determine size of specified set
	inline MStudioBox* Hitbox(i32 i, i32 set) const
	{
		MStudioHitboxSet const* s = HitboxSet(set);
		if (!s)
			return nullptr;

		return s->Hitbox(i);
	};

	// Calls through to set to get hitbox count for set
	inline i32 HitboxCount(i32 set) const
	{
		MStudioHitboxSet const* s = HitboxSet(set);
		if (!s)
			return 0;

		return s->numHitboxes;
	};

	// File local animations? and sequences
	//private:
	i32 numLocalAnim; ///< Animations/poses
	i32 localAnimIdx; ///< Animation descriptions
	i32 numLocalSeq; ///< Sequences
	i32 localSeqIdx;

	//public:
	bool SequencesAvailable() const;

	i32 GetNumSeq() const;

	i32 RelativeAnim(i32 baseSeq, i32 relAnim) const; // Maps seq local anim reference to global anim index
	i32 RelativeSeq(i32 baseSeq, i32 relSeq) const; // Maps seq local seq reference to global seq index

	//private:
	mutable i32 activityListVersion; // Initialization flag - have the sequences been indexed?
	mutable i32 eventsIndexed;

	//public:
	i32 GetSequenceActivity(i32 sequence);

	void SetSequenceActivity(i32 sequence, i32 activity);

	i32 GetActivityListVersion();

	void SetActivityListVersion(i32 version) const;

	i32 GetEventListVersion();

	void SetEventListVersion(i32 version);

	// Raw textures
	i32 numTextures;
	i32 textureIndex;

	// Raw textures search paths
	i32 numCdTextures;
	i32 cdTextureIdx;

	inline char* CdTexture(i32 i) const
	{
		return (((char*) this) + *((i32*) (((u8*) this) + cdTextureIdx) + i));
	};

	// Replaceable textures tables
	i32 numSkinRef;
	i32 numSkinFamilies;
	i32 skinIdx;

	inline i16* pSkinref(i32 i) const
	{
		return (i16*) (((u8*) this) + skinIdx) + i;
	};
	i32 numBodyParts;
	i32 bodyPartIdx;

	// Queryable attachable points
	//private:
	i32 numLocalAttachments;
	i32 localAttachmentIdx;

	//public:
	i32 GetNumAttachments() const;

	i32 GetAttachmentBone(i32 i);

	// used on my tools in hlmv, not persistant
	void SetAttachmentBone(i32 attachment, i32 bone);

	// animation node to animation node transition graph
	//private:
	i32 numLocalNodes;
	i32 localNodeIdx;
	i32 localNodeNameIdx;

	inline char* LocalNodeName(i32 node) const
	{
		(node >= 0 && node < numLocalNodes);
		return (((char*)this) + *((i32*) (((u8*)this) + localNodeNameIdx) + node));
	}

	inline u8* LocalTransition(i32 i) const
	{
		(i >= 0 && i < (numLocalNodes * numLocalNodes));
		return (u8*)(((u8*)this) + localNodeIdx) + i;
	};

	//public:
	i32 EntryNode(i32 sequence);

	i32 ExitNode(i32 sequence);

	char* NodeName(i32 node);

	i32 GetTransition(i32 from, i32 to) const;

	i32 numFlexDesc;
	i32 flexDescIdx;
	i32 numFlexControllers;
	i32 flexControllerIdx;
	i32 numFlexRules;
	i32 flexRuleIdx;
	i32 numIkChains;
	i32 ikChainIdx;
	i32 numMouths;
	i32 mouthIdx;

	//private:
	i32 numLocalPoseParameters;
	i32 localPoseParamIdx;

	//public:
	i32 GetNumPoseParameters() const;

	i32 GetSharedPoseParameter(i32 sequence, i32 localPose) const;

	i32 surfacePropIdx;

	inline char* const SurfaceProp() const
	{
		return ((char*)this) + surfacePropIdx;
	}

	// Key values
	i32 keyValueIdx;
	i32 keyValueSize;

	inline const char* KeyValueText() const
	{
		return keyValueSize != 0 ? ((char*)this) + keyValueIdx : nullptr;
	}

	i32 numLocalIkAutoplayLocks;
	i32 localIkAutoplayLockIdx;

	i32 GetNumIKAutoplayLocks() const;

	i32 CountAutoplaySequences() const;

	i32 CopyAutoplaySequences(u16* out, i32 outCount) const;

	i32 GetAutoplayList(u16** out) const;

	// The collision model mass that jay wanted
	f32 mass;
	i32 contents;

	// External animations, models, etc.
	i32 numIncludeModels;
	i32 includeModelIdx;

	// implementation specific call to get a named model
	const StudioHdr *FindModel(void** cache, char const* modelName) const;

	// Implementation specific back pointer to virtual data
	mutable void* virtualModel;
	//virtualmodel_t		GetVirtualModel() const;

	// For demand loaded animation blocks
	i32 animBlockNameIdx;

	inline char* const AnimBlockName() const
	{
		return ((char*)this) + animBlockNameIdx;
	}

	i32 numAnimBlocks;
	i32 animBlockIdx;
	mutable void* animBlockModel;

	unsigned char* GetAnimBlock(i32 i) const;

	i32 boneTableByNameIdx;

	inline const u8* GetBoneTableSortedByName() const
	{
		return (u8*)this + boneTableByNameIdx;
	}

	// Used by tools only that don't cache, but persist mdl's peer data
	// engine uses virtualModel to back link to cache pointers
	void* vertexBase;
	void* indexBase;

	// if STUDIOHDR_FLAGS_CONSTANT_DIRECTIONAL_LIGHT_DOT is set,
	// this value is used to calculate directional components of lighting
	// on static props
	unsigned char constDirectionalLightDot;

	// Set during load of mdl data to track *desired* lod configuration (not actual)
	// the *actual* clamped root lod is found in studiohwdata
	// This is stored here as a global store to ensure the staged loading matches the rendering
	unsigned char rootLOD;

	// Set in the mdl data to specify that lod configuration should only allow first numAllowRootLODs
	// to be set as root LOD:
	//	numAllowedRootLODs = 0	means no restriction, any lod can be set as root lod.
	//	numAllowedRootLODs = N	means that lod0 - lod(N-1) can be set as root lod, but not lodN or lower.
	unsigned char numAllowedRootLODs;
	unsigned char unused[1];
	i32 unused4; // Zero out if version < 47
	i32 numFlexControllerUi;
	i32 flexControllerUiIdx;
	i32 unused3[2];

	// FIXME: Remove when we up the model version. Move all fields of studiohdr2_t into studiohdr_t.
	i32 studiohdr2Idx;

	// NOTE: No room to add stuff? Up the .mdl file format version
	// [and move all fields in studiohdr2_t into studiohdr_t and kill studiohdr2_t],
	// or add your stuff to studiohdr2_t. See NumSrcBoneTransforms/SrcBoneTransform for the pattern to use.
	i32 unused2[1];

	StudioHdr() { }
private:
	// No copy constructors allowed
	StudioHdr(const StudioHdr &vOther);

	friend struct VirtualModel;
};

#endif // SDK_CSTUDIOHDR_HPP
