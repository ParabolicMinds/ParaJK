#include "g_mono.h"

static monoImport_t * mono;
static monoapihandle_t * mapi;
static mono_class * mcl;

qboolean initialized = qfalse;

/*
================================================
G_Mono Export Methods
	To be called by this program in order to
	execute managed C# code.
================================================
*/

static mono_method * KillEntity_S;
void G_Mono_KillEntity(gentity_t * ent) {
	void * params[] = { ent };
	mono->InvokeStaticMethod(KillEntity_S, params);
}

/*
================================================
*/

/*
================================================
G_Mono Import Methods
	To be called by PJKSE_G.dll in order to
	alter aspects of the Game VM.
================================================
*/

static void G_Mono_Kill(void * ent) {
	G_Kill(ent);
}

static void G_Mono_Trap_Print(mono_string * str) {
	char * prnt = mono->GetNewCharsFromString(str);
	trap->Print(prnt);
	mono->FreeMonoObject(prnt);
}

/*
================================================
*/

qboolean G_Mono_Initialize() {
	if (initialized) return qtrue;
	if (!mono)
		mono = trap->MonoCreateImport();

	if (!mapi)
		mapi = mono->Initialize("para/PJKSE_G.dll");
	if (!mapi) goto critfail;

	if (!mcl)
		mcl = mono->GetClassData(mapi, "", "GAME");
	if (!mcl) goto critfail;

	//Nothing below here should fail.

	//Setup C# Exports
	KillEntity_S = mono->GetStaticMethod(mcl, "KillEntity", 1);

	//Setup C# Imports
	mono->RegisterCMethod("GAME_IMPORT::GMono_Kill", G_Mono_Kill);
	mono->RegisterCMethod("GAME_IMPORT::GMono_Print", G_Mono_Trap_Print);

	//Done
	initialized = qtrue;
	return qtrue;


	critfail:
	//Uh oh...
	trap->Print("Mono Initialization: CRITICAL FAILURE\n");
	return qfalse;
}
