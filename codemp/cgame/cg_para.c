#include "cg_para.h"

#define parmm_len 4096
static char parmm[parmm_len];

int pjkCGCvarIntValue(const char *name) {
	memset(parmm, 0x00, parmm_len);
	trap->Cvar_VariableStringBuffer(name, parmm, parmm_len);
	return atoi(parmm);
}

float pjkCGCvarFloatValue(const char *name) {
	memset(parmm, 0x00, parmm_len);
	trap->Cvar_VariableStringBuffer(name, parmm, parmm_len);
	return strtof(parmm, NULL);
}

char * pjkCGCvarStringValue(const char * name) {
	memset(parmm, 0x00, parmm_len);
	trap->Cvar_VariableStringBuffer(name, parmm, parmm_len);
	return strdup(parmm);
}

vmPcvarfunc_t const vmCvarCGFuncs[] = {
	{ PJK_BGAME_SABER_LENGTH_CVAR,				CG_PJK_UpdateSaberLengths },
};
int const vmCvarCGFuncsLen = ARRAY_LEN(vmCvarCGFuncs);

void CG_PJK_UpdateSaberLengths () {
	clientInfo_t * cl;
	int i, j, k;
	saberInfo_t * sb;
	bladeInfo_t * bi;
	for (i=0, cl = cgs.clientinfo; i < MAX_CLIENTS; i++, cl++) {
		if (cl && cl->saber) {
			for (j=0,sb=cl->saber;j<MAX_SABERS;j++,sb++) {
				if (sb && sb->blade) {
					for (k=0,bi=sb->blade;k<MAX_BLADES;k++,bi++) {
						if (bi) {
							bi->lengthMax = bi->lengthNormalMax * pjkCGCvarFloatValue(PJK_BGAME_SABER_LENGTH_CVAR);
						}
					}
				}
			}
		}
	}
}


qhandle_t cg_pjk_golfball;
