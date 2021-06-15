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

////////////////////////////////////////////////
/// \brief Failed bit, set to true if failed during initialization
////////////////////////////////////////////////
static bool failed = false; 

Redir stdoutRedir, stderrRedir;

int __attribute__((constructor)) Startup()
{
	stdoutRedir.Create(stdout, "/mnt/ram_disk/cunnyware.out");
	stderrRedir.Create(stdout, "/mnt/ram_disk/cunnyware.err");

	try
	{
		fmt::print("Loading...\n");

		Interface::FindSymbols();
		Interface::FindInterfaces();

		SDL2::Hook();

		ColorRGBA c(255,0,0);
		cvar->ConsoleColorPrintf(ColorRGBA(0, 255, 0), "Cunnyware has been successfully loaded!\n");
	}
	catch (Exception& e)
	{
		std::cout << "\033[31m***EXCEPTION CAUGHT***\n";
		std::cout << e.what();
		std::cout << "\n***END OF EXCEPTION***\033[0m" << std::endl;

		failed = true;
	}

	return 0;
}

void __attribute__((destructor)) Shutdown()
{
	VMT::ReleaseAllVMTs();

	if (!failed)
	{
		SDL2::UnHook();
	}

	VMT::ReleaseAllVMTs();
	
	fmt::print("Cunnyware unloaded.\n");

	stdoutRedir.Release();
	stderrRedir.Release();
}
