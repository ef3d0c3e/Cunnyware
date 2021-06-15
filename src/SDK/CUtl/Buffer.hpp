#ifndef SDK_CUTL_BUFFER_HPP
#define SDK_CUTL_BUFFER_HPP

#include "Memory.hpp"
#include "Byteswap.hpp"

class CUtlBuffer : non_constructible
{
	// Brian has on his todo list to revisit this as there are issues in some cases with CUtlVector using operator = instead of copy construtor in InsertMultiple, etc.
	// The unsafe case is something like this:
	//  CUtlVector< CUtlBuffer > vecFoo;
	//
	//  CUtlBuffer buf;
	//  buf.Put( xxx );
	//  vecFoo.Insert( buf );
	//
	//  This will cause memory corruption when vecFoo is cleared
	//
	//private:
	//	// Disallow copying
	//	CUtlBuffer( const CUtlBuffer & );// { Assert( 0 ); }
	//	CUtlBuffer &operator=( const CUtlBuffer & );//  { Assert( 0 ); return *this; }

public:
	MAKE_CENUM_Q(SeekType, i32, HEAD, 0, CURRENT, 1, TAIL, 2);

	MAKE_CENUM_Q(BufferFlags, i32,
		TEXT_BUFFER, 0x1,          // Describes how get + put work (as strings, or binary)
		EXTERNAL_GROWABLE, 0x2,    // This is used w / external buffers and causes the utlbuf to switch to reallocatable memory if an overflow happens when Putting.
		CONTAINS_CRLF, 0x4,        // For text buffers only, does this contain \n or \n\r?
		READ_ONLY, 0x8,            // For external buffers; prevents null termination from happening.
		AUTO_TABS_DISABLED, 0x10); // Used to disable/enable push/pop tabs

	// Overflow functions when a get or put overflows
	typedef bool (CUtlBuffer::*UtlBufferOverflowFunc)(i32 size);
	
protected:
	// error flags
	MAKE_CENUM_Q(ErrorFlag, i32,
		PUT, 0x1,
		GET, 0x2);
	
	CUtlMemory<u8> memory;
	i32 get;
	i32 put;
	
	u8 error;
	u8 flags;
	u8 reserved;
	
	i32 tab;
	i32 maxPut;
	i32 offset;
	
	UtlBufferOverflowFunc getOverflowFunc;
	UtlBufferOverflowFunc putOverflowFunc;
	
	CByteswap byteswap;
};

#endif // SDK_CUTL_BUFFER_HPP
