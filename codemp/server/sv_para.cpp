#include "qcommon/qcommon.h"
#include "qcommon/q_para.h"
#include "sv_para.h"

void SV_PARA_INIT() {
	Cvar_Get(PARA_CVAR_THERMAL_GOLF, "1", CVAR_SERVERINFO);
	Cvar_Get(PARA_CVAR_DISRUPTOR_INSTAKILL, "1", CVAR_SERVERINFO);
}
