#include "q_shared.h"

//Damage Cvars
#define PARA_CVAR_FALLDAMAGE "para_d_falldamage"

//Force Cvars
#define PARA_CVAR_INFINITE_FORCE "para_f_infinite"

//Weapon Cvars
#define PARA_CVAR_THERMAL_GOLF "para_w_thermaldet_golf"
#define PARA_CVAR_DISRUPTOR_INSTAKILL "para_w_disrpt_instakill"

//Cmds
#define PARA_CMD_TEST "partest"

typedef struct pcvar_s {
	char*		name;
	char*		defval;
	int			cvarFlags;
} pcvar_t;

extern const pcvar_t paraCvars[];
extern const size_t paraCvarsNum;
