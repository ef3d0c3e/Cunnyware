#ifndef UTIL_SYMBOL_HPP
#define UTIL_SYMBOL_HPP

#include <map>
#include <string>
#include "Elf.hpp"
#include <deque>

template <bool x64Mode>
using mElf = ELF<x64Mode, DemanglerType::GNUCXXABI>;

template <bool x64Mode>
class Library
{
public:
	std::map<std::string, std::uintptr_t> m_symbols;
	std::string path;
	std::string name;
	std::size_t address;
	std::size_t size;

	Library(const std::string& name, const std::deque<typename mElf<x64Mode>::Segment>& segs, const std::deque<typename mElf<x64Mode>::Symbol>& symbols, const std::deque<Maps::MapEntry>& entries)
	{
		if (entries.empty())
			return;

		if (segs.size() < entries.size())
			throw Exception("Library::Library() Failed, segs.size() < entries.size() in '{}'", name);

		path = entries.front().path;
		this->name = name;

		address = entries.front().address;
		size = entries.back().address - address + entries.back().size; // Rough size
		for (const auto& s : symbols)
		{
			const auto addr = mElf<x64Mode>::Resolve(s, segs, entries);
			m_symbols.insert({s.name, addr});
		}
	}
	~Library() {}

	std::uintptr_t operator[](const std::string& key) const
	{
		const auto it = m_symbols.find(key);
		if (it == m_symbols.end())
			throw Exception("Library::operator[] Could not get symbol '{0}' in module '{1}'", key, name);

		return it->second;
	}
};

template <bool x64Mode>
class SymbolTable
{
public:
	std::map<std::string, Library<x64Mode>> m_table;

	SymbolTable() {}
	~SymbolTable() {}

	const Library<x64Mode>& operator[](const std::string& key) const
	{
		const auto it = m_table.find(key);
		if (it == m_table.end())
			throw Exception("SymbolTable::operator[] Could not get library '{}'", key);

		return it->second;
	}
};

#endif // UTIL_SYMBOL_HPP
