#ifndef SDK_IMATERIALVAR_HPP
#define SDK_IMATERIALVAR_HPP

#include "../Util/Defs.hpp"
#include "CUtl/Symbol.hpp"
#include "ITexture.hpp"
#include <cassert>

MAKE_CENUM_Q(MaterialVarType, i32,
	FLOAT, 0,
	STRING, 1,
	VECTOR, 2,
	TEXTURE, 3,
	INT, 4,
	FOURCC, 5,
	UNDEFINED, 6,
	MATRIX, 7,
	MATERIAL, 8);

MAKE_CENUM_Q(MaterialVarFlags, i32,
	DEBUG, 1<<0,
	NO_DEBUG_OVERRIDE, 1<<1,
	NO_DRAW, 1<<2,
	USE_IN_FILLRATE_MODE, 1<<3,
	VERTEXCOLOR, 1<<4,
	VERTEXALPHA, 1<<5,
	SELFILLUM, 1<<6,
	ADDITIVE, 1<<7,
	ALPHATEST, 1<<8,
	//UNUSED, 1<<9,
	ZNEARER, 1<<10,
	MODEL, 1<<11,
	FLAT, 1<<12,
	NOCULL, 1<<13,
	NOFOG, 1<<14,
	IGNOREZ, 1<<15,
	DECAL, 1<<16,
	ENVMAPSPHERE, 1<<17,//OBSOLETE
	//UNUSED, 1<<18,
	ENVMAPCAMERASPACE, 1<<19,//OBSOLETE
	BASEALPHAENVMAPMASK, 1<<20,
	TRANSLUCENT, 1<<21,
	NORMALMAPALPHAENVMAPMASK, 1<<22,
	NEEDS_SOFTWARE_SKINNING, 1<<23,//OBSOLETE
	OPAQUETEXTURE, 1<<24,
	ENVMAPMODE, 1<<25,//OBSOLETE
	SUPPRESS_DECALS, 1<<26,
	HALFLAMBERT, 1<<27,
	WIREFRAME, 1<<28,
	ALLOWALPHATOCOVERAGE, 1<<29,
	ALPHA_MODIFIED_BY_PROXY, 1<<30,
	VERTEXFOG, 1<<31);

MAKE_CENUM_Q(MaterialPropertyTypes, i32,
	NEEDS_LIGHTMAP, 0,          //bool
	OPACITY, 1,                 //int(enumMaterialPropertyOpacityTypes_t)
	REFLECTIVITY, 2,            //vec3_t
	NEEDS_BUMPED_LIGHTMAPS3, 3, //bool
);

typedef u16 MaterialVarSym;
typedef u64 FourCC;

class IMaterial;
class IMaterialVar
{
protected:
	// base data and accessors
	char* sVal;
	i32 iVal;
	Vec4 vVal;

	// member data. total = 4 bytes
	u8 type = 4;
	u8 numVectorComps = 3;
	u8 fakeMaterialVar = true;
	u8 tempIndex;
	CUtlSymbol name;

public:
	// class factory methods
	static IMaterialVar* Create(IMaterial* material, char const* key, const Mat4x4& matrix);
	static IMaterialVar* Create(IMaterial* material, char const* key, char const* val);
	static IMaterialVar* Create(IMaterial* material, char const* key, f32* val, i32 numcomps);
	static IMaterialVar* Create(IMaterial* material, char const* key, f32 val);
	static IMaterialVar* Create(IMaterial* material, char const* key, i32 val);
	static IMaterialVar* Create(IMaterial* material, char const* key);
	static void Destroy(IMaterialVar* var);
	static MaterialVarSym GetSymbol(char const* name);
	static MaterialVarSym FindSymbol(char const* name);
	static bool SymbolMatches(char const* name, MaterialVarSym symbol);
	static void DeleteUnreferencedTextures(bool enable);

	virtual ITexture* GetTextureValue() = 0;

	virtual char const* GetName() const = 0;
	virtual MaterialVarSym GetNameAsSymbol() const = 0;

	virtual void SetFloatValue(f32 val) = 0;

	virtual void SetIntValue(i32 val) = 0;

	virtual void SetStringValue(char const* val) = 0;
	virtual char const* GetStringValue() const = 0;

	// Use FourCC values to pass app-defined data structures between
	// the proxy and the shader. The shader should ignore the data if
	// its FourCC type not correct.
	virtual void SetFourCCValue(FourCC type, void* pData) = 0;
	virtual void GetFourCCValue(FourCC* type, void** ppData) = 0;

	// Vec (dim 2-4)
	virtual void SetVecValue(f32 const* val, u32 numcomps) = 0;
	virtual void SetVecValue(f32 x, f32 y) = 0;
	virtual void SetVecValue(f32 x, f32 y, f32 z) = 0;
	virtual void SetVecValue(f32 x, f32 y, f32 z, f32 w) = 0;
	virtual void GetLinearVecValue(f32* val, i32 numcomps) const = 0;

	// revisit: is this a good interface for textures?
	virtual void SetTextureValue(ITexture*) = 0;

	virtual IMaterial* GetMaterialValue() = 0;
	virtual void SetMaterialValue(IMaterial*) = 0;

	virtual bool IsDefined() const = 0;
	virtual void SetUndefined() = 0;

	// Matrix
	virtual void SetMatrixValue(const Mat4x4& matrix) = 0;
	virtual const Mat4x4& GetMatrixValue() = 0;
	virtual bool MatrixIsIdentity() const = 0;

	// Copy....
	virtual void CopyFrom(IMaterialVar* materialVar) = 0;

	virtual void SetValueAutodetectType(char const* val) = 0;

	virtual IMaterial* GetOwningMaterial() = 0;

	//set just 1 component
	virtual void SetVecComponentValue(f32 fVal, i32 nComponent) = 0;

protected:
	virtual i32 GetIntValueInternal() const = 0;
	virtual f32 GetFloatValueInternal() const = 0;
	virtual f32 const* GetVecValueInternal() const = 0;
	virtual void GetVecValueInternal(f32* val, i32 numcomps) const = 0;
	virtual i32 VectorSizeInternal() const = 0;

public:
	inline MaterialVarType GetType() const
	{
		return (MaterialVarType)type;
	}

	inline bool IsTexture() const
	{
		return type == MaterialVarType::TEXTURE;
	}

	inline operator ITexture*()
	{
		return GetTextureValue();
	}

	// NOTE: Fast methods should only be called in thread-safe situations
	inline i32 GetIntValueFast() const
	{
		// Set methods for float and vector update this
		return iVal;
	}

	inline f32 GetFloatValueFast() const
	{
		return vVal[0];
	}

	inline f32 const* GetVecValueFast() const
	{
		return reinterpret_cast<const f32*>(&vVal);
	}

	inline void GetVecValueFast(f32* val, i32 numcomps) const
	{
		assert((numcomps > 0) && (numcomps <= 4));
		for (int i = 0; i < numcomps; i++)
		{
			val[i] = vVal[i];
		}
	}

	inline i32 VectorSizeFast() const
	{
		return numVectorComps;
	}

	inline i32 GetIntValue() const
	{
		return GetIntValueInternal();
	}

	inline f32 GetFloatValue() const
	{
		return GetFloatValueInternal();
	}

	inline f32 const* GetVecValue() const
	{
		return GetVecValueInternal();
	}

	inline void GetVecValue(f32* val, i32 numcomps) const
	{
		return GetVecValueInternal(val, numcomps);
	}

	inline i32 VectorSize() const
	{
		return VectorSizeInternal();
	}

private:
	inline void SetTempIndex(i32 index)
	{
		tempIndex = index;
	}
};

#endif // SDK_IMATERIALVAR_HPP
