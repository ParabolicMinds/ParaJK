#pragma GCC diagnostic ignored "-Wmissing-braces"
#include "q_para.h"

//==================
//GAME
//==================
const pcvar_t pjk_g_cvars[] = {
	{ PJK_GAME_INFINITE_FORCE_CVAR,					PJK_GAME_INFINITE_FORCE_DVAL,					PJK_GAME_INFINITE_FORCE_FLAG,							qtrue },
	{ PJK_GAME_THERMAL_GOLF_CVAR,					PJK_GAME_THERMAL_GOLF_DVAL,						PJK_GAME_THERMAL_GOLF_FLAG,								qtrue },
	{ PJK_GAME_DISRUPTOR_INSTAKILL_CVAR,			PJK_GAME_DISRUPTOR_INSTAKILL_DVAL,				PJK_GAME_DISRUPTOR_INSTAKILL_FLAG,						qtrue },
	{ PJK_GAME_DETPACK_LAUNCH_CVAR,					PJK_GAME_DETPACK_LAUNCH_DVAL,					PJK_GAME_DETPACK_LAUNCH_FLAG,							qtrue },
	{ PJK_GAME_MASSMOD_CVAR,						PJK_GAME_MASSMOD_DVAL,							PJK_GAME_MASSMOD_FLAG,									qtrue },
	{ PJK_GAME_ROCKETVEL_CVAR,						PJK_GAME_ROCKETVEL_DVAL,						PJK_GAME_ROCKETVEL_FLAG,								qtrue },
	{ PJK_GAME_ROCKETSIZE_CVAR,						PJK_GAME_ROCKETSIZE_DVAL,						PJK_GAME_ROCKETSIZE_FLAG,								qtrue },
	{ PJK_GAME_ROCKETALTVELMOD_CVAR,				PJK_GAME_ROCKETALTVELMOD_DVAL,					PJK_GAME_ROCKETALTVELMOD_FLAG,							qtrue },
};
const size_t pjk_g_num = ARRAY_LEN(pjk_g_cvars);

//==================
//CLIENT GAME
//==================
const pcvar_t pjk_cg_cvars[] = {

};
const size_t pjk_cg_num = ARRAY_LEN(pjk_cg_cvars);

//==================
//BOTH CLIENT AND SERVER GAME
//==================
const pcvar_t pjk_bg_cvars[] = {
	{ PJK_BGAME_FALLDAMAGE_CVAR,					PJK_BGAME_FALLDAMAGE_DVAL,						PJK_BGAME_FALLDAMAGE_FLAG,								qtrue },
	{ PJK_BGAME_INFINITE_JUMP_CVAR,					PJK_BGAME_INFINITE_JUMP_DVAL,					PJK_BGAME_INFINITE_JUMP_FLAG,							qtrue },
};
const size_t pjk_bg_num = ARRAY_LEN(pjk_bg_cvars);

//==================
//SERVER NON-GAME
//==================
const pcvar_t pjk_sv_cvars[] = {
	{ PJK_SERV_SENDTARGETS_CVAR,					PJK_SERV_SENDTARGETS_DVAL,						PJK_SERV_SENDTARGETS_FLAG,								qfalse },
};
const size_t pjk_sv_num = ARRAY_LEN(pjk_sv_cvars);
