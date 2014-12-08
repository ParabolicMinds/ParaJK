#ifndef G_MONO_H
#define G_MONO_H

#include "g_local.h"
#include "mono/mono_api_vm.h"

//================================
//G_Mono API Methods
//================================
qboolean G_MonoApi_Initialize();
void G_MonoApi_Shutdown();
void G_MonoApi_Frame();
void G_MonoApi_LoadMapCSPack(char const * packname);
//================================
//////////////////////////////////
//================================
//G_Mono Export Methods
//================================
void G_MonoApi_MapEntry(gentity_t * activator, char const * entrytag);
//================================

#endif //G_MONO_H
