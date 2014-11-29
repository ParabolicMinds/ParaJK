#include "g_local.h"
#include "g_para.h"
#include "pcommon/q_para.h"

typedef struct cvarTable_s {
	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	void		(*update)( void );
	uint32_t	cvarFlags;
	qboolean	trackChange; // announce if value changes
} cvarTable_t;

typedef struct vmPcvarG_s {
	vmCvar_t	vm;
	void		(*update)( void );
} vmPcvarG_t;

#define XCVAR_DECL
	#include "g_xcvar.h"
#undef XCVAR_DECL

static const cvarTable_t gameCvarTable[] = {
	#define XCVAR_LIST
		#include "g_xcvar.h"
	#undef XCVAR_LIST
};


static const size_t gameCvarTableSize = ARRAY_LEN( gameCvarTable );

static vmPcvarG_t * pjkVm;

void G_RegisterCvars( void ) {
	size_t i, j;

	if (pjkVm) {
		free(pjkVm);
	}
	if (pbgcvars) {
		free(pbgcvars);
	}
	//Initialize pjkVm size to hold all G and BG Cvars.
	pjkVm = calloc(pjk_g_num + pjk_bg_num, sizeof(vmPcvarG_t));
	//Initialize the local BG cvar store
	pbgcvars = calloc(pjk_bg_num, sizeof(pbgcvar_t));

	vmPcvarG_t * pcvm;
	pcvar_t const * pcv;
	vmPcvarfunc_t const * vmpcf;

	for ( i=0, pcvm = pjkVm, pcv = pjk_g_cvars; i < pjk_g_num; i++, pcv++, pcvm++ ) {
		trap->Cvar_Register( &pcvm->vm, pcv->name, pcv->defval, pcv->flags );
		for (j=0, vmpcf=vmCvarGFuncs; j < vmCvarGFuncsLen; j++, vmpcf++) {
			if (!strcmp(vmpcf->name, pcv->name)) {
				pcvm->update = vmpcf->update;
			}
		}
	}
	pbgcvar_t * pbgc;
	for ( pcv = pjk_bg_cvars, pbgc = pbgcvars; i < pjk_g_num + pjk_bg_num; i++, pcv++, pcvm++, pbgc++ ) {
		trap->Cvar_Register( &pcvm->vm, pcv->name, pcv->defval, pcv->flags );
		for (j=0, vmpcf=vmCvarGFuncs; j < vmCvarGFuncsLen; j++, vmpcf++) {
			if (!strcmp(vmpcf->name, pcv->name)) {
				pcvm->update = vmpcf->update;
			}
		}
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

	vmPcvarG_t * pcvm;
	pcvar_t const * pcv;
	for ( i=0, pcvm = pjkVm, pcv = pjk_g_cvars; i < pjk_g_num; i++, pcv++, pcvm++ ) {
		int modCount = pcvm->vm.modificationCount;
		trap->Cvar_Update( &pcvm->vm );
		if ( pcvm->vm.modificationCount != modCount ) {
			if (pcvm->update) pcvm->update();
			if ( pcv->announce )
				trap->SendServerCommand( -1, va("print \"Server: PJK G Cvar %s changed to %s\n\"", pcv->name, pcvm->vm.string ) );
		}
	}
	pbgcvar_t * pbgc;
	for ( pcv = pjk_bg_cvars, pbgc = pbgcvars; i < pjk_g_num + pjk_bg_num; i++, pcv++, pcvm++, pbgc++ ) {
		int modCount = pcvm->vm.modificationCount;
		trap->Cvar_Update( &pcvm->vm );
		if ( pcvm->vm.modificationCount != modCount ) {
			if (pbgc->value) free(pbgc->value);
			pbgc->value = strdup(pcvm->vm.string);
			if (pcvm->update) pcvm->update();
			if ( pcv->announce )
				trap->SendServerCommand( -1, va("print \"Server: PJK BG Cvar %s changed to %s\n\"", pcv->name, pcvm->vm.string ) );
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
