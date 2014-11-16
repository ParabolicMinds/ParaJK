#include "g_para.h"
#include "g_local.h"
#include "qcommon/q_para.h"
#include "bg_public.h"

qboolean Para_Cvar_Disrupt_IK() {
	return trap->Cvar_VariableIntegerValue(PARA_CVAR_DISRUPTOR_INSTAKILL);
}

qboolean Para_Cvar_ThermalGolf() {
	return trap->Cvar_VariableIntegerValue(PARA_CVAR_THERMAL_GOLF);
}

qboolean Para_Cvar_InfForce() {
	return trap->Cvar_VariableIntegerValue(PARA_CVAR_INFINITE_FORCE);
}

qboolean Para_Cvar_FallDamage() {
	return trap->Cvar_VariableIntegerValue(PARA_CVAR_FALLDAMAGE);
}
