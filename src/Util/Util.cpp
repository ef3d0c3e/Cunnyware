#include "Util.hpp"
#include <link.h>
#include <unistd.h>
#include <string.h>
#include <codecvt>
#include <locale>
#include "VMT.hpp"

std::string Demangle(const char* name)
{
	std::unique_ptr<char, void (*)(void*)> own // Get readable type name
		(abi::__cxa_demangle(name, nullptr, nullptr, nullptr), std::free);
	return own != nullptr ? own.get() : name;
}

Elf64_Word GetProtectionFlags(uintptr_t address)
{
	static Elf64_Word flags = 0;
	static std::uintptr_t addr = address;

	dl_iterate_phdr([](struct dl_phdr_info* info, std::size_t, void*)
	{
		std::uintptr_t startingAddr = 0;
		std::uintptr_t endingAddr = 0;

		for (Elf64_Half i = 0; i < info->dlpi_phnum; i++)
		{
			const ElfW(Phdr) *hdr = &info->dlpi_phdr[i];
			if (hdr->p_memsz)
			{
				startingAddr = info->dlpi_addr + hdr->p_vaddr;
				endingAddr = startingAddr + hdr->p_memsz;
				if (startingAddr <= addr && endingAddr >= addr)
					flags |= hdr->p_flags;
			}
		}

		return 0;
	}, nullptr);

	return flags;
}

long pageSize = sysconf(_SC_PAGESIZE);
std::pair<std::uintptr_t, std::size_t> getMinimumPage(std::uintptr_t addr, std::size_t len)
{
	std::uintptr_t ret = (addr / pageSize) * pageSize;

	std::size_t n = (addr - ret + len);
	if (n % pageSize)
		n /= pageSize, ++n;
	else
		n /= pageSize;
	
	return {ret, n*pageSize};
}

std::pair<std::uintptr_t, std::size_t> GetLibraryInformation(const std::string& library)
{
	struct ModuleInfo
	{
		const char* library = nullptr;
		std::uintptr_t address = 0;
		std::size_t size = 0;
	};
	const static std::vector<ModuleInfo> libraries = []()
	{
		std::vector<ModuleInfo> vec;

		dl_iterate_phdr([](struct dl_phdr_info* info, std::size_t, void* data)
		{
			reinterpret_cast<std::vector<ModuleInfo>*>(data)->push_back(
			ModuleInfo{
				.library = info->dlpi_name,
				.address = info->dlpi_addr + info->dlpi_phdr[0].p_vaddr,
				.size = info->dlpi_phdr[0].p_memsz,
			});

			return 0;
		}, &vec);
		
		return vec;
	}();

	for (const auto& current : libraries)
	{
		if (!strcasestr(current.library, library.c_str()))
			continue;

		return {current.address, current.size};
	}

	return {0, 0};
}

inline bool Compare(const u8* data, const std::u8string_view& pattern, const std::string_view& mask)
{
	std::size_t i;
	for (i = 0; i < mask.size(); ++i)
		if (mask[i] == 'x' && pattern[i] != data[i])
			return false;

	return i == mask.size();
}

std::uintptr_t FindPattern(std::uintptr_t address, std::size_t size, const std::u8string_view& pattern, const std::string_view& mask)
{
	for (std::uintptr_t i = 0; i < size; i++)
		if (Compare(reinterpret_cast<const u8*>(address + i), pattern, mask))
			return (std::uintptr_t)(address + i);
	return 0;
}

std::uintptr_t FindPatternInModule(const std::string& moduleNamme, const std::u8string_view& pattern, const std::string_view& mask)
{
	std::uintptr_t base;
	std::size_t size;

	if (std::tie(base, size) = GetLibraryInformation(moduleNamme); base == 0)
		throw Exception("FindPatternInModule({}, ...) could not get library information", moduleNamme);

	const std::uintptr_t ret = FindPattern(base, size, pattern, mask);
	if (ret == 0)
		throw Exception("FindPatternInModule({}, ...) could not find pattern", moduleNamme);

	return ret;
}

std::string codepointToUtf8(char32_t codepoint)
{
	try
	{
		std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
		return convert.to_bytes(&codepoint, &codepoint+1);
	}
	catch (...)
	{
		return "RANGE_ERROR";
	}
}

inline bool Compare_(const u8* pData, const std::u8string_view& pattern, const std::string_view& szMask)
{

	/*for (; *szMask; ++szMask, ++pData, ++bMask)
		if (*szMask == 'x' && *pData != *bMask)
			return false;

	return (*szMask) == 0;
	*/

	std::size_t i;
	for (i = 0; i < szMask.size(); ++i)
		if (szMask[i] == 'x' && pattern[i] != pData[i])
			return false;

	return i == szMask.size();
}

std::uintptr_t FindPattern_(std::uintptr_t dwAddress, std::size_t dwLen, const std::u8string_view& pattern, const std::string_view& szMask)
{
	for (uintptr_t i = 0; i < dwLen; i++)
		if (Compare_((u8*)(dwAddress + i), pattern, szMask))
			return (std::uintptr_t)(dwAddress + i);

	return 0;
}

uintptr_t FindPatternInModule_(const std::string& moduleName, const std::u8string_view& pattern, const std::string_view& szMask)
{
	std::uintptr_t base;
	std::size_t size;

	if (std::tie(base, size) = GetLibraryInformation(moduleName); base == 0)
		throw Exception("FindPatternInModule({}, ...) could not get library information", moduleName);

	uintptr_t ret = FindPattern_(base, size, pattern, szMask);
	return ret;
}
