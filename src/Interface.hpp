#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#include "Util/Symbol.hpp"
////////////////////////////////////////////////
/// \brief Table containing all symbols
////////////////////////////////////////////////
extern SymbolTable<true> symbols;

#include "SDK/SDK.hpp"
extern ICvar* cvar;
extern IEngineClient* engine;
extern IInputSystem* inputSystem;
extern IInputInternal* inputInternal;
extern ILauncherMgr* launcherMgr;

#include "Util/VMT.hpp"
extern VMT* inputInternalVMT;
extern VMT* launcherMgrVMT;

namespace Interface
{
////////////////////////////////////////////////
/// \brief Finds all the loaded symbols used by the program
////////////////////////////////////////////////
void FindSymbols();

////////////////////////////////////////////////
/// \brief Finds all CS:GO's interfaces
////////////////////////////////////////////////
void FindInterfaces();

////////////////////////////////////////////////
/// \brief Hooks VMT's
////////////////////////////////////////////////
void HookVMTs();
}

#endif // INTERFACE_HPP
