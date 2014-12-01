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

/*
========================================
Exception Handler
	Should be called in every Export method.
========================================
*/
static char * mono_exception = NULL;
static void HandleMonoException() {
	if (mono_exception) {
		trap->Print(va("Mono Error: %s\n", mono_exception));
		mono->FreeMonoObject(mono_exception);
		mono_exception = NULL;
	}
}
/*
========================================
*/

static mono_method * TestMono_M;
void G_MonoApi_TestMono(gentity_t * ent) {
	void * params[] = { ent };
	mono->InvokeStaticMethod(TestMono_M, params, &mono_exception);
	HandleMonoException();
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
	if (!ent) return;
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
		mcl = mono->GetClassData(mapi, "", "GAME_INTERNAL_IMPORT");
	if (!mcl) goto critfail;

	//Nothing below here should fail.

	//Setup C# Exports
	TestMono_M = mono->GetStaticMethod(mcl, "Test", 1);

	//Setup C# Imports
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_Kill", G_Mono_Kill);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_Print", G_Mono_Trap_Print);

	//Done
	initialized = qtrue;
	return qtrue;


	critfail:
	//Uh oh...
	trap->Print("Mono Initialization: CRITICAL FAILURE\n");
	return qfalse;
}
