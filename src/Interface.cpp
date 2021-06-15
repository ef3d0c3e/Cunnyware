#include "Interface.hpp"
#include "Hooks/Hooks.hpp"
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>

using namespace std::literals;

SymbolTable<true> symbols;
void Interface::FindSymbols()
{
	symbols = SymbolTable<true>();

	std::map<std::string, std::deque<Maps::MapEntry>> maps;

	Maps::ParseMaps([&maps](Maps::MapEntry&& ent)
	{
		if (ent.path.empty())
			return;
		const auto it = maps.find(ent.path);
		if (it == maps.end())
			maps.insert({ent.path, std::deque<Maps::MapEntry>{ent}});
		else
			it->second.push_back(std::move(ent));
	});

	for (const auto& library : maps)
	{
		auto getName = [](const std::string& path) // Get a readable name
		{
			const auto pos = path.find("Counter-Strike Global Offensive/"); // Path relative to current directory
			if (pos == std::string::npos || pos+32 >= path.size())
				return path;

			return path.substr(pos + 32);
		};

		const auto name = getName(library.first);
		if (name.find(".so") == std::string::npos)
			continue;
		
		static constexpr std::array<std::string_view, 24> wantedLibraries // c++20 constexpr std::string when
		{
			"launcher_client.so"sv,
			"filesystem_stdio_client.so"sv,
			"engine_client.so"sv,
			"inputsystem_client.so"sv,
			"vphysics_client.so"sv,
			"materialsystem_client.so"sv,
			"datacache_client.so"sv,
			"studiorender_client.so"sv,
			"soundemittersystem_client.so"sv,
			"vscript_client.so"sv,
			"vguimatsurface_client.so"sv,
			"vgui2_client.so"sv,
			"panoramauiclient_client.so"sv,
			"panorama_client.so"sv,
			"shaderapidx9_client.so"sv,
			"localize_client.so"sv,
			"panorama_text_pango_client.so"sv,
			"stdshader_dx9_client.so"sv,
			"matchmaking_client.so"sv,
			"client_client.so"sv,
			"server_client.so"sv,
			"scenefilecache_client.so"sv,
			"serverbrowser_client.so"sv,
			"libSDL2-2.0.so.0"sv,
		};

		for (const auto& wanted : wantedLibraries)
		{
			if (name.size() < wanted.size() || std::strncmp(&name.c_str()[name.size()-wanted.size()], std::string(wanted).c_str(), wanted.size()) != 0)
				continue;

			int fd;
			fd = open(library.first.c_str(), O_RDONLY);
			struct stat st;
			if (fstat(fd, &st) < 0)
				throw Exception("Interface::FindSymbols() stat failed on '{}'", library.first);


			void* mem = mmap(NULL, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
			if (mem == MAP_FAILED)
				throw Exception("Interface::FindSymbols() mmap failed");

			if (is64Bit((std::uintptr_t)mem))
			{
				std::deque<mElf<true>::Symbol> symbolList;
				mElf<true>::GetSymbols((std::uintptr_t)mem, [&symbolList](mElf<true>::Symbol&& s)
				{
					symbolList.push_back(std::move(s));
				});
				std::deque<mElf<true>::Segment> segs;
				mElf<true>::GetSegments((std::uintptr_t)mem, [&segs](mElf<true>::Segment&& s)
				{
					segs.push_back(std::move(s));
				});

				if (symbolList.empty())
				{
					munmap(mem, st.st_size);
					break;
				}

				symbols.m_table.insert(std::make_pair(name, Library<true>(name, segs, symbolList, library.second)));
			}

			munmap(mem, st.st_size);
			break;
		}
	}
}

ICvar* cvar = nullptr;
IEngineClient* engine = nullptr;
IInputSystem* inputSystem = nullptr;
IInputInternal* inputInternal = nullptr;

void Interface::FindInterfaces()
{
	cvar = GetInterface<ICvar>("./bin/linux64/materialsystem_client.so", "VEngineCvar");
	engine = GetInterface<IEngineClient>("./bin/linux64/engine_client.so", "VEngineClient");
	inputSystem = GetInterface<IInputSystem>("./bin/linux64/inputsystem_client.so", "InputSystemVersion");
	inputInternal = GetInterface<IInputInternal>("./bin/linux64/vgui2_client.so", "VGUI_InputInternal");
}

VMT* inputInternalVMT = nullptr;

void Interface::HookVMTs()
{
	inputInternalVMT = new VMT(inputInternal);
	inputInternalVMT->HookVM(Hooks::SetKeyCodeState, 92);
	inputInternalVMT->HookVM(Hooks::SetMouseCodeState, 93);
	inputInternalVMT->ApplyVMT();
}