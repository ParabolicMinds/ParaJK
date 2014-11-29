#include "cg_local.h"
#include "pcommon/q_para.h"
#include "cg_para.h"

//
// Cvar callbacks
//

static void CG_SVRunningChange( void ) {
	cgs.localServer = sv_running.integer;
}

static void CG_ForceModelChange( void ) {
	int i;

	for ( i=0; i<MAX_CLIENTS; i++ ) {
		const char *clientInfo = CG_ConfigString( CS_PLAYERS+i );

		if ( !VALIDSTRING( clientInfo ) )
			continue;

		CG_NewClientInfo( i, qtrue );
	}
}

static void CG_TeamOverlayChange( void ) {
	// If team overlay is on, ask for updates from the server.  If its off,
	// let the server know so we don't receive it
	if ( cg_drawTeamOverlay.integer > 0 && cgs.gametype >= GT_SINGLE_PLAYER)
		trap->Cvar_Set( "teamoverlay", "1" );
	else
		trap->Cvar_Set( "teamoverlay", "0" );
}


//
// Cvar table
//

typedef struct cvarTable_s {
	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	void		(*update)( void );
	uint32_t	cvarFlags;
} cvarTable_t;

typedef struct vmPcvarCG_s {
	vmCvar_t	vm;
	void		(*update)( void );
} vmPcvarCG_t;

#define XCVAR_DECL
	#include "cg_xcvar.h"
#undef XCVAR_DECL

static const cvarTable_t cvarTable[] = {
	#define XCVAR_LIST
		#include "cg_xcvar.h"
	#undef XCVAR_LIST
};
static const size_t cvarTableSize = ARRAY_LEN( cvarTable );

static vmPcvarCG_t * pjkVm;

void CG_RegisterCvars( void ) {
	size_t i, j;

	if (pjkVm) {
		free(pjkVm);
	}
	if (pbgcvars) {
		free(pbgcvars);
	}

	//Initialize pjkVm size to hold all CG and BG Cvars.
	pjkVm = calloc(pjk_cg_num + pjk_bg_num, sizeof(vmPcvarCG_t));
	//Initialize the local BG cvar store
	pbgcvars = calloc(pjk_bg_num, sizeof(pbgcvar_t));

	vmPcvarCG_t * pcvm;
	pcvar_t const * pcv;
	vmPcvarfunc_t const * vmpcf;

	for ( i=0, pcvm = pjkVm, pcv = pjk_cg_cvars; i < pjk_cg_num; i++, pcv++, pcvm++ ) {
		trap->Cvar_Register( &pcvm->vm, pcv->name, pcv->defval, pcv->flags );
		for (j=0, vmpcf=vmCvarCGFuncs; j < vmCvarCGFuncsLen; j++, vmpcf++) {
			if (!strcmp(vmpcf->name, pcv->name)) {
				pcvm->update = vmpcf->update;
			}
		}
	}
	pbgcvar_t * pbgc;
	for ( pcv = pjk_bg_cvars, pbgc = pbgcvars; i < pjk_cg_num + pjk_bg_num; i++, pcv++, pcvm++, pbgc++ ) {
		trap->Cvar_Register( &pcvm->vm, pcv->name, pcv->defval, pcv->flags );
		for (j=0, vmpcf=vmCvarCGFuncs; j < vmCvarCGFuncsLen; j++, vmpcf++) {
			if (!strcmp(vmpcf->name, pcv->name)) {
				pcvm->update = vmpcf->update;
			}
		}
		pbgc->name = pcv->name;
		pbgc->value = pjkCGCvarStringValue(pcv->name);
	}

	const cvarTable_t *cv = NULL;

	for ( i=0, cv=cvarTable; i<cvarTableSize; i++, cv++ ) {
		trap->Cvar_Register( cv->vmCvar, cv->cvarName, cv->defaultString, cv->cvarFlags );
		if ( cv->update )
			cv->update();
	}
}

void CG_UpdateCvars( void ) {
	size_t i = 0;

	vmPcvarCG_t * pcvm;
	pcvar_t const * pcv;
	for ( i=0, pcvm = pjkVm, pcv = pjk_cg_cvars; i < pjk_cg_num; i++, pcv++, pcvm++ ) {
		int modCount = pcvm->vm.modificationCount;
		trap->Cvar_Update( &pcvm->vm );
		if ( pcvm->vm.modificationCount != modCount ) {
			if (pcvm->update) pcvm->update();
		}
	}
	pbgcvar_t * pbgc;
	for ( pcv = pjk_bg_cvars, pbgc = pbgcvars; i < pjk_cg_num + pjk_bg_num; i++, pcv++, pcvm++, pbgc++ ) {
		int modCount = pcvm->vm.modificationCount;
		trap->Cvar_Update( &pcvm->vm );
		if ( pcvm->vm.modificationCount != modCount ) {
			if (pcvm->update) pcvm->update();
			if (pbgc->value) free(pbgc->value);
			pbgc->value = strdup(pcvm->vm.string);
		}
	}

	const cvarTable_t *cv = NULL;

	for ( i=0, cv=cvarTable; i<cvarTableSize; i++, cv++ ) {
		if ( cv->vmCvar ) {
			int modCount = cv->vmCvar->modificationCount;
			trap->Cvar_Update( cv->vmCvar );
			if ( cv->vmCvar->modificationCount != modCount ) {
				if ( cv->update )
					cv->update();
			}
		}
	}
}

void CG_ShutdownCvars( void ) {
	if (pjkVm) {
		free(pjkVm);
	}
	pjkVm = NULL;
}
