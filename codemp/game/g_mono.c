#include "g_mono.h"

static monoImport_t * mono;
static monoapihandle_t * mapi;
static mono_class * mcl;

qboolean initialized = qfalse;

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
void G_MonoApi_Internal_Initialize(char const * name) {
	mono_string * packname = mono->CharPtrToString(name);
	void * params[] = { packname };
	mono->InvokeStaticMethod(Init_M, params, &mono_exception);
	HandleMonoException();
}

static mono_method * MapInit_M;
void G_MonoApi_Internal_MapInitialize() {
	void * params[] = {};
	mono->InvokeStaticMethod(MapInit_M, params, &mono_exception);
	HandleMonoException();
}

static mono_method * Frame_M;
void G_MonoApi_Frame(void) {
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

static mono_method * Reset_M;
void G_MonoApi_Reset(void) {
	void * params[] = { };
	mono->InvokeStaticMethod(Reset_M, params, &mono_exception);
	HandleMonoException();
}

static mono_method * ChatE_M;
void G_MonoApi_ChatEvent(gentity_t * sender, char const * msg) {
	mono_string * msgm = mono->CharPtrToString(msg);
	void * params[] = { sender, msgm };
	mono->InvokeStaticMethod(ChatE_M, params, &mono_exception);
	HandleMonoException();
}

static mono_method * Entity_M;
void G_MonoApi_Internal_EntityEntry(
		const char * entrytag,
		gentity_t * self,
		gentity_t * activator,
		mono_entitypass_t targets,
		mono_entitypass_t targets2,
		mono_entitypass_t targets3,
		mono_entitypass_t targets4) {

	mono_string * mtag = mono->CharPtrToString(entrytag);
	void * params[] = { mtag, self, activator, &targets.count, targets.ptrarry, &targets2.count, targets2.ptrarry, &targets3.count, targets3.ptrarry, &targets4.count, targets4.ptrarry};
	mono->InvokeStaticMethod(Entity_M, params, &mono_exception);
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
	VectorCopy( ent->s.origin, ent->r.currentOrigin );
	VectorClear(ent->s.pos.trDelta);
	ent->s.pos.trTime = level.time;
	ent->s.pos.trType = TR_STATIONARY;
	ent->think = 0;
	ent->nextthink = level.time;
	trap->UnlinkEntity((sharedEntity_t *)ent);
	trap->LinkEntity((sharedEntity_t *)ent);
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
}
static void G_Mono_MoveStop(gentity_t * ent) {
	VectorCopy(ent->s.origin, ent->s.pos.trBase);
	VectorCopy(ent->s.origin, ent->r.currentOrigin);
	VectorClear(ent->s.pos.trDelta);
	ent->s.pos.trType = TR_STATIONARY;
}

static mono_string * G_Mono_GetModelName(gentity_t * ent) {
	if (ent->model)
		return mono->CharPtrToString(ent->model);
	else return NULL;
}
static void G_Mono_SetModelName(gentity_t * ent, mono_string * str) {
	char * model = mono->GetNewCharsFromString(str);
	ent->model = strdup(model);
	mono->FreeMonoObject(model);
	ent->s.modelindex = G_ModelIndex(ent->model);
}
static mono_string * G_Mono_GetTargetname(gentity_t * ent) {
	if (ent->targetname)
		return mono->CharPtrToString(ent->targetname);
	else return NULL;
}
static void G_Mono_SetTargetname(gentity_t * ent, mono_string * str) {
	char * tname = mono->GetNewCharsFromString(str);
	ent->targetname = strdup(tname);
	mono->FreeMonoObject(tname);
}
static void G_Mono_UseTarget(gentity_t * self, gentity_t * activator, mono_string * mtarget) {
	char * target = mono->GetNewCharsFromString(mtarget);
	G_UseTargets2(self, activator, target);
	mono->FreeMonoObject(target);
}
static int G_Mono_GetEntitiesByName(mono_string * mtarget, gentity_t * * buffer, int bufptrlen, int substring) {
	char * targetn = mono->GetNewCharsFromString(mtarget);
	gentity_t * t = NULL;
	int c = 0;
	if (substring) {
		while ((t = G_SubstringFind(t, FOFS(targetname), targetn)) != NULL && c < bufptrlen) {
			buffer[c] = t;
			c++;
		}
	} else {
		while ((t = G_Find(t, FOFS(targetname), targetn)) != NULL && c < bufptrlen) {
			buffer[c] = t;
			c++;
		}
	}
	mono->FreeMonoObject(targetn);
	return c;
}
static int G_Mono_GetSkinIndex(gentity_t * ent) {
	return ent->s.modelSkinIndex;
}
static void G_Mono_SetSkinIndex(gentity_t * ent, int newIndex) {
	ent->s.modelSkinIndex = newIndex;
}
static void * G_Mono_GetModelScale(gentity_t * ent) {
	return ent->s.modelScale;
}
static void G_Mono_SetModelScale(gentity_t * ent, float X, float Y, float Z) {
	VectorSet(ent->s.modelScale, X, Y, Z);
}
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
static int G_Mono_GetType(gentity_t * ent) {
	return ent->s.eType;
}
static void G_Mono_SetType(gentity_t * ent, int newtype) {
	ent->s.eType = newtype;
}
static void G_Mono_CenterPrint(mono_string * message, int clinum, int svprnt) {
	char * msg = mono->GetNewCharsFromString(message);
	if (svprnt)
		trap->SendServerCommand( clinum, va("cps \"%s\"", msg ));
	else
		trap->SendServerCommand( clinum, va("cp \"%s\"", msg ));
	mono->FreeMonoObject(msg);
}
static int G_Mono_GetClientNum(gentity_t * cli) {
	return cli->playerState ? cli->playerState->clientNum : -1;
}

//SERVER
static void G_Mono_Trap_Print(mono_string * str) {
	char * prnt = mono->GetNewCharsFromString(str);
	trap->Print(prnt);
	mono->FreeMonoObject(prnt);
}
static int G_Mono_OpenRead(mono_string * path, void * handle) {
	char * pathc = mono->GetNewCharsFromString(path);
	int len = trap->FS_Open(pathc, handle, FS_READ);
	mono->FreeMonoObject(pathc);
	return len;
}

/*
================================================================================================
*/

qboolean G_MonoApi_Initialize() {
	if (initialized) return qtrue;
	if (!mono)
		mono = trap->MonoCreateImport();

	if (!mapi)
		mapi = mono->Initialize("para/pjkse_game.dll");
	if (!mapi) goto critfail;

	if (!mcl)
		mcl = mono->GetClassData(mapi, "", "GAME_INTERNAL_IMPORT");
	if (!mcl) goto critfail;

	//Nothing below here should fail.

	//Setup C# Imports
	Init_M = mono->GetStaticMethod(mcl, "GMono_Initialize", 1);
	MapInit_M = mono->GetStaticMethod(mcl, "GMono_MapInit", 0);
	Frame_M = mono->GetStaticMethod(mcl, "GMono_Frame", 1);
	Shutdown_M = mono->GetStaticMethod(mcl, "GMono_Shutdown", 0);
	Entity_M = mono->GetStaticMethod(mcl, "GMono_EntityEntry", 11);
	Reset_M = mono->GetStaticMethod(mcl, "GMono_Reset", 0);
	ChatE_M = mono->GetStaticMethod(mcl, "GMono_ChatEvent", 2);

	//Setup C# Exports
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_Spawn", G_Mono_Spawn);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_Free", G_Mono_Free);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_GetOriginPtr", G_Mono_GetPositionPtr);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_SetOrigin", G_Mono_SetOrigin);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_MoveTo", G_Mono_MoveTo);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_MoveStop", G_Mono_MoveStop);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_GetModel", G_Mono_GetModelName);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_SetModel", G_Mono_SetModelName);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_GetTargetname", G_Mono_GetTargetname);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_SetTargetname", G_Mono_SetTargetname);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_UseTarget", G_Mono_UseTarget);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_UseEntity", GlobalUse);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_Kill", G_Mono_Kill);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_IsNPC", G_Mono_IsNPC);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_IsPlayer", G_Mono_IsPlayer);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_GetHealth", G_Mono_GetHealth);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_SetHealth", G_Mono_SetHealth);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_GetSkinIndex", G_Mono_GetSkinIndex);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_SetSkinIndex", G_Mono_SetSkinIndex);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_GetModelScale", G_Mono_GetModelScale);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_SetModelScale", G_Mono_SetModelScale);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_GetEntityType", G_Mono_GetType);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_SetEntityType", G_Mono_SetType);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_Print", G_Mono_Trap_Print);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_CenterPrint", G_Mono_CenterPrint);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_GetClientNum", G_Mono_GetClientNum);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_GetEntitiesByName", G_Mono_GetEntitiesByName);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_FS_Open", G_Mono_OpenRead);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_FS_Read", trap->FS_Read);
	mono->RegisterCMethod("GAME_INTERNAL_EXPORT::GMono_FS_Close", trap->FS_Close);

	//Done
	initialized = qtrue;
	return qtrue;


	critfail:
	//Uh oh...
	trap->Print("Mono Initialization: CRITICAL FAILURE\n");
	return qfalse;
}

void G_MonoApi_Shutdown() {
	G_MonoApi_Internal_Shutdown();
	mono->ShutdownAPIHandle(mapi);
	mono->FreeVMImport(mono);
}

#define CSPACK_BUFLEN 65535


void G_MonoApi_MapEntry(
		const char * entrytag,
		gentity_t * self,
		gentity_t * activator,
		mono_entitypass_t targets,
		mono_entitypass_t targets2,
		mono_entitypass_t targets3,
		mono_entitypass_t targets4) {

	G_MonoApi_Internal_EntityEntry(entrytag, self, activator, targets, targets2, targets3, targets4);
}
