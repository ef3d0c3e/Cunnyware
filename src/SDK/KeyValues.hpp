#ifndef SDK_KEYVALUES_HPP
#define SDK_KEYVALUES_HPP

#include "Color.hpp"

class KeyValues : non_constructible
{
public:
	//
	// AutoDelete class to automatically free the keyvalues.
	// Simply construct it with the keyvalues you allocated and it will free them when falls out of scope.
	// When you decide that keyvalues shouldn't be deleted call Assign(NULL) on it.
	// If you constructed AutoDelete(NULL) you can later assign the keyvalues to be deleted with Assign(pKeyValues).
	//

	MAKE_CENUM_Q(Types, i32,
		NONE, 0,
		STRING, 1,
		INT, 2,
		FLOAT, 3,
		PTR, 4,
		WSTRING, 5,
		COLOR, 6,
		UINT64, 7,
		COMPILED_INT_BYTE, 8, // hack to collapse 1 byte ints in the compiled format
		COMPILED_INT_0, 9, // hack to collapse 0 in the compiled format
		COMPILED_INT_1, 10, // hack to collapse 1 in the compiled format
		NUMTYPES, 11,
	);
	
	MAKE_CENUM_Q(MergeKeyValuesOp, i32,
		ALL, 0,
		UPDATE, 1, // update values are copied into storage, adding new keys to storage or updating existing ones
		DELETE, 2, // update values specify keys that get deleted from storage
		BORROW, 3, // update values only update existing keys in storage, keys in update that do not exist in storage are discarded
	);

	u32 keyName = 24; // keyname is a symbol defined in KeyValuesSystem
	u32 keyNameCaseSensitive1 = 8; // 1st part of case sensitive symbol defined in KeyValueSystem

	// These are needed out of the union because the API returns string pointers
	char* value;
	wchar_t* wsValue;

	// we don't delete these
	union
	{
		i32 iValue;
		f32 fValue;
		void* pValue;
		std::array<u8, 4> cValue;
	};

	char dataType;
	char hasEscapeSequences; // true, if while parsing this KeyValue, Escape Sequences are used (default false)
	u16 keyNameCaseSensitive2; // 2nd part of case sensitive symbol defined in KeyValueSystem;

	KeyValues* peer; // pointer to next key in list
	KeyValues* sub; // pointer to Start of a new sub key list
	KeyValues* chain; // Search here if it's not in our list
};

#endif // SDK_KEYVALUES_HPP
