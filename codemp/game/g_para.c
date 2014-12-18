#include "g_para.h"

#define parmm_len 4096
static char parmm[parmm_len];

int pjkGCvarIntValue(const char *name) {
	return trap->Cvar_VariableIntegerValue(name);
}

float pjkGCvarFloatValue(const char *name) {
	memset(parmm, 0x00, parmm_len);
	trap->Cvar_VariableStringBuffer(name, parmm, parmm_len);
	return strtof(parmm, NULL);
}

char * pjkGCvarStringValue(const char * name) {
	memset(parmm, 0x00, parmm_len);
	trap->Cvar_VariableStringBuffer(name, parmm, parmm_len);
	return strdup(parmm);
}

vmPcvarfunc_t const vmCvarGFuncs[] = {
	{ PJK_BGAME_SABER_LENGTH_CVAR,				G_PJK_UpdateSaberLengths },
	{ PJK_BGAME_JETPACK_FUEL_CVAR,				G_PJK_UpdateJetpackTanks },
	{ PJK_BGAME_THERMAL_GOLF_CVAR,				G_PJK_UpdateThermalGolfMode },
};
int const vmCvarGFuncsLen = ARRAY_LEN(vmCvarGFuncs);

gentity_t * par_golfBalls[MAX_CLIENTS];

int shaderRemapsThisFrame = 0;
