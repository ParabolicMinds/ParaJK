#include "g_mono.h"

static monoImport_t * mono;
static monoapihandle_t * mapi;
static mono_class * mcl;

qboolean initialized = qfalse;

void G_Mono_FinishMove(gentity_t * ent) {
	VectorCopy(ent->s.origin, ent->s.pos.trBase);
	VectorClear(ent->s.pos.trDelta);
	ent->s.pos.trType = TR_STATIONARY;
}

/*
================================================================================================
G_Mono Import Methods
	To be called by this program in order to
	execute managed C# code.
================================================================================================
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

static mono_method * Init_M;
void G_MonoApi_Internal_Init(void) {
	void * params[] = { };
	mono->InvokeStaticMethod(Init_M, params, &mono_exception);
	HandleMonoException();
}

static mono_method * Frame_M;
void G_MonoApi_Internal_Frame(void) {
	void * params[] = { &level.time };
	mono->InvokeStaticMethod(Frame_M, params, &mono_exception);
	HandleMonoException();
}

static mono_method * Shutdown_M;
void G_MonoApi_Internal_Shutdown(void) {
	void * params[] = { };
	mono->InvokeStaticMethod(Shutdown_M, params, &mono_exception);
	HandleMonoException();
}

static mono_method * TestMono_M;
void G_MonoApi_TestMono(gentity_t * ent) {
	void * params[] = { ent };
	mono->InvokeStaticMethod(TestMono_M, params, &mono_exception);
	HandleMonoException();
}

/*
================================================================================================
*/

/*
================================================================================================
G_Mono Export Methods
	To be called by PJKSE_G.dll in order to
	alter aspects of the Game VM.
================================================================================================
*/

//ENTITY MANAGEMENT
static void * G_Mono_Spawn() {
	gentity_t * ent = G_Spawn();
	ent->s.eType = ET_MOVER;
	ent->r.svFlags = SVF_BROADCAST;
	trap->LinkEntity((sharedEntity_t *)ent);
	return ent;
}
static void G_Mono_Free(gentity_t * ent) {
	if (!ent) return;
	G_FreeEntity(ent);
}

//General
static void * G_Mono_GetPositionPtr(gentity_t * ent) {
	return (ent->client && ent->client->sess.sessionTeam != TEAM_SPECTATOR) ? ent->r.currentOrigin : ent->s.origin;
}
static void G_Mono_SetOrigin(gentity_t * ent, float X, float Y, float Z) {
	vec3_t newPos = {X, Y, Z};
	VectorCopy(newPos, ent->s.origin);
	VectorCopy( ent->s.origin, ent->s.pos.trBase );
	VectorClear(ent->s.pos.trDelta);
	ent->s.pos.trTime = level.time;
	ent->s.pos.trType = TR_STATIONARY;
	ent->think = 0;
	ent->nextthink = 0;
}
static void G_Mono_MoveTo(gentity_t * ent, float X, float Y, float Z, float timespan) {
	vec3_t newPos = {X, Y, Z};

	VectorCopy( ent->s.origin, ent->s.pos.trBase );
	VectorSubtract(newPos, ent->s.origin, ent->s.pos.trDelta);
	float spMod = VectorLength(ent->s.pos.trDelta)/(timespan / 1000.f);
	VectorNormalize(ent->s.pos.trDelta);
	VectorScale(ent->s.pos.trDelta, spMod, ent->s.pos.trDelta);

	ent->s.pos.trTime = level.time;
	ent->s.pos.trType = TR_LINEAR;

	VectorCopy(newPos, ent->s.origin);

	ent->think = G_Mono_FinishMove;
	ent->nextthink = level.time + timespan;
}

static mono_string * G_Mono_GetModelName(gentity_t * ent) {
	return mono->CharPtrToString(ent->model);
}
static void G_Mono_SetModelName(gentity_t * ent, mono_string * str) {
	char * model = mono->GetNewCharsFromString(str);
	ent->model = strdup(model);
	mono->FreeMonoObject(model);
	ent->s.modelindex = G_ModelIndex(ent->model);
}

//NPC/PLAYER
static void G_Mono_Kill(gentity_t * ent) {
	if (!ent || !ent->client) return;
	G_Kill(ent);
}
static int G_Mono_IsNPC(gentity_t * ent) {
	if (ent->NPC) return 1;
	return 0;
}
static int G_Mono_IsPlayer(gentity_t * ent) {
	if (ent->client) return 1;
	return 0;
}
static int G_Mono_GetHealth(gentity_t * ent) {
	return ent->health;
}
static void G_Mono_SetHealth(gentity_t * ent, int newHealth) {
	ent->health = newHealth;
	if (ent->client) ent->client->ps.stats[STAT_HEALTH] = newHealth;
}
//SERVER
static void G_Mono_Trap_Print(mono_string * str) {
	char * prnt = mono->GetNewCharsFromString(str);
	trap->Print(prnt);
	mono->FreeMonoObject(prnt);
}

/*
================================================================================================
*/

qboolean G_MonoApi_Initialize() {
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

	//Setup C# Imports
	Init_M = mono->GetStaticMethod(mcl, "GMono_Init", 0);
	Frame_M = mono->GetStaticMethod(mcl, "GMono_Frame", 1);
	Shutdown_M = mono->GetStaticMethod(mcl, "GMono_Shutdown", 0);
	TestMono_M = mono->GetStaticMethod(mcl, "Test", 1);

	//Setup C# Exports
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_Spawn", G_Mono_Spawn);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_Free", G_Mono_Free);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_GetOriginPtr", G_Mono_GetPositionPtr);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_SetOrigin", G_Mono_SetOrigin);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_MoveTo", G_Mono_MoveTo);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_GetModel", G_Mono_GetModelName);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_SetModel", G_Mono_SetModelName);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_Kill", G_Mono_Kill);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_IsNPC", G_Mono_IsNPC);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_IsPlayer", G_Mono_IsPlayer);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_GetHealth", G_Mono_GetHealth);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_SetHealth", G_Mono_SetHealth);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_Print", G_Mono_Trap_Print);

	//Done
	initialized = qtrue;
	G_MonoApi_Internal_Init();
	return qtrue;


	critfail:
	//Uh oh...
	trap->Print("Mono Initialization: CRITICAL FAILURE\n");
	return qfalse;
}

void G_MonoApi_Shutdown() {
	G_MonoApi_Internal_Shutdown();
	mono->FreeMonoObject(mcl);
	mcl = NULL;
	mono->ShutdownAPIHandle(mapi);
	mono->FreeVMImport(mono);
}

void G_MonoApi_Frame() {
	G_MonoApi_Internal_Frame();
}
