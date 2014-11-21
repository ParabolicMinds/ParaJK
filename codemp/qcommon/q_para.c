#pragma GCC diagnostic ignored "-Wmissing-braces"
#include "q_para.h"

#define NULL_VMCVART {0, 0, 0, 0, 0}

//==================
//GAME
//==================
pcvar_t pjk_g_cvars[] = {
	{ NULL_VMCVART, PARA_CVAR_INFINITE_FORCE,			"0",			CVAR_SERVERINFO|CVAR_ARCHIVE,						qtrue},
	{ NULL_VMCVART, PARA_CVAR_THERMAL_GOLF,				"0",			CVAR_SERVERINFO|CVAR_ARCHIVE,						qtrue},
	{ NULL_VMCVART, PARA_CVAR_DISRUPTOR_INSTAKILL,		"0",			CVAR_SERVERINFO|CVAR_ARCHIVE,						qtrue},
	{ NULL_VMCVART, PARA_CVAR_DETPACK_LAUNCH,			"0",			CVAR_SERVERINFO|CVAR_ARCHIVE,						qtrue},
};
const size_t pjk_g_num = ARRAY_LEN(pjk_g_cvars);

//==================
//CLIENT GAME
//==================
pcvar_t pjk_cg_cvars[] = {

};
const size_t pjk_cg_num = ARRAY_LEN(pjk_cg_cvars);

//==================
//BOTH CLIENT AND SERVER GAME
//==================
pcvar_t pjk_bg_cvars[] = {
	{ NULL_VMCVART, PARA_CVAR_FALLDAMAGE,				"1",			CVAR_SYSTEMINFO|CVAR_ARCHIVE,						qtrue},
	{ NULL_VMCVART, PARA_CVAR_INFINITE_JUMP,			"0",			CVAR_SYSTEMINFO|CVAR_ARCHIVE,						qtrue},
	{ NULL_VMCVART, PARA_CVAR_MASSMODIFIER,				"1.0f",			CVAR_SYSTEMINFO|CVAR_ARCHIVE,						qtrue},
};
const size_t pjk_bg_num = ARRAY_LEN(pjk_bg_cvars);

//==================
//SERVER NON-GAME
//==================
pcvar_t pjk_sv_cvars[] = {
	{ NULL_VMCVART, PJK_SERV_SENDTARGETS,				"1",			CVAR_ARCHIVE,										qfalse},
};
const size_t pjk_sv_num = ARRAY_LEN(pjk_sv_cvars);
