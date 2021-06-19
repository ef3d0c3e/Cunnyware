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
extern IBaseClientDll* client;
extern IClientMode* clientMode;
extern CEngineVGui* engineVGui;
extern IGameEventManager* gameEvent;
extern IMaterialSystem* material;
extern IVModelRender* modelRender;
extern IVPanel* panel;
extern IEngineSound* sound;

#include "Util/VMT.hpp"
extern VMT* inputInternalVMT;
extern VMT* launcherMgrVMT;
extern VMT* clientVMT;
extern VMT* clientModeVMT;
extern VMT* engineVGuiVMT;
extern VMT* gameEventVMT;
extern VMT* materialVMT;
extern VMT* modelRenderVMT;
extern VMT* panelVMT;
extern VMT* soundVMT;

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
