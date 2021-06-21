#ifndef SDK_RECV_HPP
#define SDK_RECV_HPP

#include "Defs.hpp"

struct RecvProp;

struct DVariant
{
	union {
		f32 f;
		i64 i;
		char* string;
		void* data;
		Vec3 vector;
		i64 ilong;
	};

	i32 type;
};

struct CRecvProxyData
{
	const RecvProp* recvProp;
	DVariant value;
	i32 element;
	i32 objectID;
};

struct RecvTable
{
	RecvProp* pProps;
	i32 props;
	void* decoder;
	char* netTableName;
	bool initialized;
	bool inMainList;
};

typedef void (*RecvVarProxyFn)(const CRecvProxyData* data, void* object, void* out);

struct RecvProp
{
	char* varName;
	i32 recvType;
	i32 flags;
	i32 stringBufferSize;
	bool insideArray;
	const void* extraData;
	RecvProp* arrayProp;
	void* arrayLengthProxy;
	RecvVarProxyFn proxyFn;
	void* dataTableProxyFn;
	RecvTable* dataTable;
	i32 offset;
	i32 elementStride;
	i32 elements;
	const char* parentArrayPropName;
};

class IClientNetworkable;
typedef IClientNetworkable* (*CreateClientClassFn)(i32 entnum, i32 serialNum);
typedef IClientNetworkable* (*CreateEventFn)();

class ClientClass
{
public:
	CreateClientClassFn createFn;
	CreateEventFn* createEventFn;
	char* networkName;
	RecvTable* recvTable;
	ClientClass* next;
	EClassIds classID;
};

#endif // SDK_RECV_HPP
