#include "g_para.h"
#include "g_local.h"
#include "qcommon/q_para.h"
#include "bg_public.h"

void Para_Update_Cvars() {
	//Game
	para_w_disrpt_ik = trap->Cvar_VariableIntegerValue(PARA_CVAR_DISRUPTOR_INSTAKILL);
	para_w_thermalgolf = trap->Cvar_VariableIntegerValue(PARA_CVAR_THERMAL_GOLF);
	para_f_infforce = trap->Cvar_VariableIntegerValue(PARA_CVAR_INFINITE_FORCE);
	para_w_detpacklaunch = trap->Cvar_VariableIntegerValue(PARA_CVAR_DETPACK_LAUNCH);
	//BGame
	para_d_falldamage = trap->Cvar_VariableIntegerValue(PARA_CVAR_FALLDAMAGE);
	para_f_infjump =  trap->Cvar_VariableIntegerValue(PARA_CVAR_INFINITE_JUMP);
}

qboolean para_w_disrpt_ik;
qboolean para_w_thermalgolf;
qboolean para_f_infforce;
qboolean para_w_detpacklaunch;
