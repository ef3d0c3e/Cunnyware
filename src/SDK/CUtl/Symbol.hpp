#ifndef SDK_CUTL_SYMBOL_HPP
#define SDK_CUTL_SYMBOL_HPP

#include "RBTree.hpp"
#include "Vector.hpp"
#include <array>

typedef u16 UtlSymId;

class CUtlSymbol : public non_constructible
{
public:
	UtlSymId id;
};

class CUtlSymbolTable : public non_constructible
{
	typedef u16 hashDecoration;

	class CStringPoolIndex
	{
	public:
		inline CStringPoolIndex()
		{
		}

		inline CStringPoolIndex(u16 _pool, u16 _offset)
		: pool(_pool), offset(_offset)
		{}

		inline bool operator==(const CStringPoolIndex& other) const
		{
			return pool == other.pool && offset == other.offset;
		}

		u16 pool; // Index into m_StringPools.
		u16 offset; // Index into the string pool.
	};

	class CLess
	{
	public:
		CLess(i32 ignored = 0)
		{} // permits default initialization to NULL in CUtlRBTree
		bool operator!() const
		{
			return false;
		}
	};

	// Stores the symbol lookup
	class CTree : public CUtlRBTree<CStringPoolIndex, u16, CLess>
	{
	};

	struct StringPool
	{
		i32 totalLen; // How large is
		i32 spaceUsed;
		std::array<char, 1> data;
	};

	CTree lookup;

	bool insensitive;
	mutable u16 userSearchStringHash;
	mutable const char* userSearchString;

	// stores the string data
	CUtlVector<StringPool*> stringPools;
};

#endif // SDK_CUTL_SYMBOL_HPP
