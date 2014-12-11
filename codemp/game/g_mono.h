#ifndef G_MONO_H
#define G_MONO_H

#include "g_local.h"
#include "mono/mono_api_vm.h"

typedef struct mono_entitypass_s {
	int count;
	gentity_t * * ptrarry;
} mono_entitypass_t;

//================================
//G_Mono API Methods
//================================
qboolean G_MonoApi_Initialize();
void G_MonoApi_Shutdown();
//================================
//////////////////////////////////
//================================
//G_Mono Export Methods
//================================
void G_MonoApi_Frame();
void G_MonoApi_Internal_Initialize(char const * packname);
void G_MonoApi_Reset();
void G_MonoApi_ChatEvent(gentity_t * sender, char const * msg);
void G_MonoApi_MapEntry(
		const char * entrytag,
		gentity_t * self,
		gentity_t * activator,
		mono_entitypass_t targets,
		mono_entitypass_t targets2,
		mono_entitypass_t targets3,
		mono_entitypass_t targets4);
//================================

#endif //G_MONO_H
