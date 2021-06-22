#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <fstream>
#include <unistd.h>
#include <fmt/format.h>
#include "Hooks/Hooks.hpp"
#include <unistd.h>
#include <stdio.h>
#include "SDK/C_BaseEntity.hpp"

////////////////////////////////////////////////
/// \brief Failed bit, set to true if failed during initialization
////////////////////////////////////////////////
static bool failed = false; 

Redir stdoutRedir, stderrRedir;

void Start()
{
	stdoutRedir.Create(stdout, "/mnt/ram_disk/cunnyware.out");
	stderrRedir.Create(stderr, "/mnt/ram_disk/cunnyware.err");

	try
	{
		fmt::print("Loading...\n");

		Interface::FindSymbols();
		Interface::FindInterfaces();
		Offsets::GetNetVarOffsets();

		SDL2::Hook();
		Interface::HookVMTs();

		cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255, 255), "Cunnyware has been successfully loaded!\n");

		//auto idx = VMTIndex<IClientEntity>(&IClientEntity::GetAllClasses);
		//cvar->ConsoleColorPrintf(ColorRGBA(255, 255, 255, 255), "index =  %d\n", idx);
	}
	catch (Exception& e)
	{
		std::cout << "\033[31m***EXCEPTION CAUGHT***\n";
		std::cout << e.what();
		std::cout << "\n***END OF EXCEPTION***\033[0m" << std::endl;

		failed = true;
	}
}

int __attribute__((constructor)) Startup()
{
	std::thread(Start).detach();

	return 0;
}

void __attribute__((destructor)) Shutdown()
{
	cvar->FindVar("cl_mouseenable")->SetValue(1);

	try
	{
		if (!failed)
		{
			SDL2::UnHook();
			VMT::ReleaseAllVMTs();
		}
	}
	catch (Exception& e)
	{
		std::cout << "\033[31m***EXCEPTION CAUGHT***\n";
		std::cout << e.what();
		std::cout << "\n***END OF EXCEPTION***\033[0m" << std::endl;
	}

	fmt::print("Cunnyware unloaded.\n");

	stdoutRedir.Release();
	stderrRedir.Release();
}
