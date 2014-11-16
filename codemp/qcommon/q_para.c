#include "q_para.h"

const pcvar_t paraCvars[] = {
	//NAME								DEFAULT			FLAGS
	{PARA_CVAR_INFINITE_FORCE,			"0",			CVAR_SERVERINFO},
	{PARA_CVAR_THERMAL_GOLF,			"0",			CVAR_SERVERINFO},
	{PARA_CVAR_DISRUPTOR_INSTAKILL,		"0",			CVAR_SERVERINFO},
	{PARA_CVAR_FALLDAMAGE,				"1",			CVAR_SERVERINFO},
};
const size_t paraCvarsNum = ARRAY_LEN(paraCvars);
