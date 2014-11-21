#include "g_local.h"
#include "g_para.h"
#include "qcommon/q_para.h"

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

void G_RegisterCvars( void ) {
	size_t i = 0;

	pcvar_t * pcv = NULL;
	for (pcv=pjk_g_cvars; i < pjk_g_num; i++, pcv++) {
		trap->Cvar_Register( &pcv->vmc, pcv->name, pcv->defval, pcv->cvarFlags );
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
	pcvar_t * pcv = NULL;

	for ( i=0, pcv=pjk_g_cvars; i<pjk_g_num; i++, pcv++ ) {
		int modCount = pcv->vmc.modificationCount;
		trap->Cvar_Update( &pcv->vmc );
		if ( pcv->vmc.modificationCount != modCount ) {
			if ( pcv->announce )
				trap->SendServerCommand( -1, va("print \"Server: %s changed to %s\n\"", pcv->name, pcv->vmc.string ) );
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
					trap->SendServerCommand( -1, va("print \"Server: %s changed to %s\n\"", cv->cvarName, cv->vmCvar->string ) );
			}
		}
	}
}
