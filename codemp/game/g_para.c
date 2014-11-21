#include "g_para.h"
#include "g_local.h"
#include "qcommon/q_para.h"
#include "bg_public.h"

#define PARA_STRBUF_LEN 128

static char * parmm = NULL;

static float Cvar_FloatValue(char const * name) {
	trap->Cvar_VariableStringBuffer(name, parmm, PARA_STRBUF_LEN);
	float f = strtof(parmm, NULL);
	if (f == HUGE_VALF) para_m_massmodifier = FLT_MAX;
	if (f == -HUGE_VALF) para_m_massmodifier = FLT_MIN;
	return f;
}

void Para_Update_Cvars() {
	parmm = calloc(PARA_STRBUF_LEN, sizeof(char));
	//Game
	para_w_disrpt_ik = trap->Cvar_VariableIntegerValue(PARA_CVAR_DISRUPTOR_INSTAKILL);
	para_w_thermalgolf = trap->Cvar_VariableIntegerValue(PARA_CVAR_THERMAL_GOLF);
	para_f_infforce = trap->Cvar_VariableIntegerValue(PARA_CVAR_INFINITE_FORCE);
	para_w_detpacklaunch = trap->Cvar_VariableIntegerValue(PARA_CVAR_DETPACK_LAUNCH);
	//BGame
	para_d_falldamage = trap->Cvar_VariableIntegerValue(PARA_CVAR_FALLDAMAGE);
	para_f_infjump =  trap->Cvar_VariableIntegerValue(PARA_CVAR_INFINITE_JUMP);
	para_m_massmodifier = Cvar_FloatValue(PARA_CVAR_MASSMODIFIER);
	free(parmm);
}

gentity_t * par_golfBalls[MAX_CLIENTS];

qboolean para_w_disrpt_ik;
qboolean para_w_thermalgolf;
qboolean para_f_infforce;
qboolean para_w_detpacklaunch;
