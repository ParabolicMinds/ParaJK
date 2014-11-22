#include "q_shared.h"

//Damage Cvars
#define PARA_CVAR_FALLDAMAGE			"pjk_dam_fall"

//Motion Cvars
#define PARA_CVAR_MASSMODIFIER			"pjk_mov_massmod"
#define PARA_CVAR_INFINITE_JUMP			"pjk_mov_infjump"

//Force Cvars
#define PARA_CVAR_INFINITE_FORCE		"pjk_frc_infinite"

//Weapon Cvars
#define PARA_CVAR_THERMAL_GOLF			"pjk_wep_thermalgolf"
#define PARA_CVAR_DISRUPTOR_INSTAKILL	"pjk_wep_ikdistruptor"
#define PARA_CVAR_DETPACK_LAUNCH		"pjk_wep_detpacklm"

//Server Cvars
#define PJK_SERV_SENDTARGETS			"pjk_sv_sendtargets"

//Cmds
#define PARA_CMD_TEST "partest"

typedef struct pcvar_s {
	char*		name;
	char*		defval;
	int			cvarFlags;
	qboolean	announce; //Only used with Game and BGame vars
} pcvar_t;

extern const pcvar_t pjk_g_cvars[];
extern const size_t pjk_g_num;
extern const pcvar_t pjk_cg_cvars[];
extern const size_t pjk_cg_num;
extern const pcvar_t pjk_bg_cvars[];
extern const size_t pjk_bg_num;
extern const pcvar_t pjk_sv_cvars[];
extern const size_t pjk_sv_num;
