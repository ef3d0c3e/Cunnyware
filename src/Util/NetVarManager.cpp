#include "NetVarManager.hpp"
#include "../Interface.hpp"

#include <sstream>
#include <fstream>

std::vector<RecvTable*> NetVarManager::GetTables()
{
	std::vector<RecvTable*> tables;

	ClientClass* clientClass = client->GetAllClasses();
	if (!clientClass)
		return std::vector<RecvTable*>();

	while (clientClass)
	{
		RecvTable* recvTable = clientClass->recvTable;
		tables.push_back(recvTable);

		clientClass = clientClass->next;
	}
	
	return tables;
}

RecvTable* NetVarManager::GetTable(std::vector<RecvTable*> tables, const char* tableName)
{
	if (tables.empty())
		return nullptr;

	for (unsigned long i = 0; i < tables.size(); i++)
	{
		RecvTable* table = tables[i];

		if (!table)
			continue;

		if (strcasecmp(table->netTableName, tableName) == 0)
			return table;
	}

	return nullptr;
}

i32  NetVarManager::GetOffset(std::vector<RecvTable*> tables, const char* tableName, const char* propName)
{
	i32 offset = GetProp(tables, tableName, propName);
	if (!offset)
		return 0;

	return offset;
}

i32 NetVarManager::GetProp(std::vector<RecvTable*> tables, const char* tableName, const char* propName, RecvProp** prop)
{
	RecvTable* recvTable = GetTable(tables, tableName);
	if (!recvTable)
		return 0;

	i32 offset = GetProp(tables, recvTable, propName, prop);
	if (!offset)
		return 0;

	return offset;
}

i32 NetVarManager::GetProp(std::vector<RecvTable*> tables, RecvTable* recvTable, const char* propName, RecvProp** prop)
{
	i32 extraOffset = 0;

	for (i32 i = 0; i < recvTable->props; ++i) {
		RecvProp* recvProp = &recvTable->pProps[i];
		RecvTable* child = recvProp->dataTable;

		if (child && (child->props > 0))
		{
			i32 tmp = GetProp(tables, child, propName, prop);
			if (tmp)
				extraOffset += (recvProp->offset + tmp);
		}

		if (strcasecmp(recvProp->varName, propName))
			continue;

		if (prop)
			*prop = recvProp;

		return (recvProp->offset + extraOffset);
	}

	return extraOffset;
}

std::string NetVarManager::DumpTable(RecvTable* table, i32 depth)
{
	std::string pre("");
	std::stringstream ss;

	for (i32 i = 0; i < depth; i++)
		pre.append("\t");

	ss << pre << table->netTableName << "\n";

	for (i32 i = 0; i < table->props; i++) {
		RecvProp* prop = &table->pProps[i];
		if (!prop)
			continue;

		std::string varName(prop->varName);

		if (varName.find("baseclass") == 0 || varName.find("0") == 0 || varName.find("1") == 0 || varName.find("2") == 0)
			continue;

		ss << pre << "\t" << varName << " [0x" << std::hex << prop->offset << "]\n";

		if (prop->dataTable)
			ss << DumpTable(prop->dataTable, depth + 1);
	}

	return ss.str();
}
/*
bool NetVarManager::HookProp(const char* tableName, const char* propName, RecvVarProxyFn f)
{
	RecvProp* recvProp = 0;
	NetVarManager::GetProp(NetVarManager::GetTables(), tableName, propName, &recvProp);
	if (!recvProp)
		return false;

	recvProp->m_ProxyFn = f;

	return true;
}
*/

void NetVarManager::DumpNetvars()
{
	std::stringstream ss;

	for (ClientClass* pClass = client->GetAllClasses(); pClass != nullptr; pClass = pClass->next)
	{
		RecvTable* table = pClass->recvTable;
		ss << NetVarManager::DumpTable(table, 0);
	}

	std::string netvarsPath = "/tmp/netvars.txt";

	std::ofstream(netvarsPath) << ss.str();
}
