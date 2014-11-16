#include "qcommon/qcommon.h"
#include "qcommon/q_para.h"
#include "sv_para.h"


//Cvars are setup here. This is also where default values are set.
void SV_PARA_INIT() {
	pcvar_t const * cv;
	int cvn;
	for (cvn = 0; cvn <= paraCvarsNum; cv = &paraCvars[cvn], cvn++) {
		Cvar_Get(cv->name, cv->defval, cv->cvarFlags);
	}
}
