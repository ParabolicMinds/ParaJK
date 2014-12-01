#ifndef G_MONO_H
#define G_MONO_H

#include "g_local.h"
#include "mono/mono_api_vm.h"
//================================
//G_Mono API Methods
//================================
qboolean G_Mono_Initialize();
//================================
//////////////////////////////////
//================================
//G_Mono Export Methods
//================================
void G_MonoApi_TestMono(gentity_t *);
//================================

#endif //G_MONO_H
