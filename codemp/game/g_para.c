#include "g_para.h"
#include "g_local.h"
#include "qcommon/q_para.h"

qboolean Para_Disrupt_IK() {
	return trap->Cvar_VariableIntegerValue(PARA_CVAR_DISRUPTOR_INSTAKILL);
}

qboolean Para_ThermalGolf() {
	return trap->Cvar_VariableIntegerValue(PARA_CVAR_THERMAL_GOLF);
}
