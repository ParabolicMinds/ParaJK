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

gentity_t * par_golfBalls[MAX_CLIENTS];
