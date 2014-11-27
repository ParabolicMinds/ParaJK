#include "g_local.h"
#include "g_para.h"
#include "pcommon/q_para.h"

//
// Cvar callbacks
//

/*
static void CVU_Derpity( void ) {
	// ...
}
*/


//
// Cvar table
//

typedef struct cvarTable_s {
	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	void		(*update)( void );
	uint32_t	cvarFlags;
	qboolean	trackChange; // announce if value changes
} cvarTable_t;

#define XCVAR_DECL
	#include "g_xcvar.h"
#undef XCVAR_DECL

static const cvarTable_t gameCvarTable[] = {
	#define XCVAR_LIST
		#include "g_xcvar.h"
	#undef XCVAR_LIST
};

static const size_t gameCvarTableSize = ARRAY_LEN( gameCvarTable );

static vmCvar_t * pjkVm;

void G_RegisterCvars( void ) {
	size_t i = 0;

	if (pjkVm) {
		free(pjkVm);
	}
	if (pbgcvars) {
		free(pbgcvars);
	}
	pjkVm = calloc(pjk_g_num + pjk_bg_num, sizeof(vmCvar_t));
	pbgcvars = calloc(pjk_bg_num, sizeof(pbgcvar_t));
	vmCvar_t * pcvm;
	pcvar_t const * pcv;
	for ( i=0, pcvm = pjkVm, pcv = pjk_g_cvars; i < pjk_g_num; i++, pcv++, pcvm++ ) {
		trap->Cvar_Register( pcvm, pcv->name, pcv->defval, pcv->flags );
	}
	pbgcvar_t * pbgc;
	for ( pcv = pjk_bg_cvars, pbgc = pbgcvars; i < pjk_g_num + pjk_bg_num; i++, pcv++, pcvm++, pbgc++ ) {
		trap->Cvar_Register( pcvm, pcv->name, pcv->defval, pcv->flags );
		pbgc->name = pcv->name;
		pbgc->value = pjkGCvarStringValue(pcv->name);
	}

	cvarTable_t const * cv = NULL;

	for ( i=0, cv=gameCvarTable; i<gameCvarTableSize; i++, cv++ ) {
		trap->Cvar_Register( cv->vmCvar, cv->cvarName, cv->defaultString, cv->cvarFlags );
		if ( cv->update )
			cv->update();
	}
}

void G_UpdateCvars( void ) {
	size_t i = 0;

	vmCvar_t * pcvm;
	pcvar_t const * pcv;
	for ( i=0, pcvm = pjkVm, pcv = pjk_g_cvars; i < pjk_g_num; i++, pcv++, pcvm++ ) {
		int modCount = pcvm->modificationCount;
		trap->Cvar_Update( pcvm );
		if ( pcvm->modificationCount != modCount ) {
			if ( pcv->announce )
				trap->SendServerCommand( -1, va("print \"Server: PJK G Cvar %s changed to %s\n\"", pcv->name, pcvm->string ) );
		}
	}
	pbgcvar_t * pbgc;
	for ( pcv = pjk_bg_cvars, pbgc = pbgcvars; i < pjk_g_num + pjk_bg_num; i++, pcv++, pcvm++, pbgc++ ) {
		int modCount = pcvm->modificationCount;
		trap->Cvar_Update( pcvm );
		if ( pcvm->modificationCount != modCount ) {
			if (pbgc->value) free(pbgc->value);
			pbgc->value = strdup(pcvm->string);
			if ( pcv->announce )
				trap->SendServerCommand( -1, va("print \"Server: PJK BG Cvar %s changed to %s\n\"", pcv->name, pcvm->string ) );
		}
	}

	const cvarTable_t *cv = NULL;

	for ( i=0, cv=gameCvarTable; i<gameCvarTableSize; i++, cv++ ) {
		if ( cv->vmCvar ) {
			int modCount = cv->vmCvar->modificationCount;
			trap->Cvar_Update( cv->vmCvar );
			if ( cv->vmCvar->modificationCount != modCount ) {
				if ( cv->update )
					cv->update();

				if ( cv->trackChange )
					trap->SendServerCommand( -1, va("print \"Server: Base Cvar %s changed to %s\n\"", cv->cvarName, cv->vmCvar->string ) );
			}
		}
	}
}

void G_ShutdownCvars( void ) {
	if (pjkVm) {
		free(pjkVm);
	}
	pjkVm = NULL;
}
