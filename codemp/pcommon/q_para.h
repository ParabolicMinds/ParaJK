#ifndef Q_PARA_H
#define Q_PARA_H

#include "qcommon/q_shared.h"

#ifdef __cplusplus
extern "C" {
#endif

/*How2CreateParaJKCvar
 *
 * Step 1:
 *	Create the 3 defines for CVAR, DVAL (Default Value), and FLAG (Cvar Flags) under the respective part of the game: Game, CGame, BGame, Server, and Client
 *
 * Step 2:
 *	Go to q_para.c and complete its entry into its respective prototype table.
 *	"announce" is whether or not the server prints a message whenenver it is changed, only matters for Game and BGame, otherwise ignored.
 *
 * Step 3:
 *	Use Cvars in the code by reffering to their CVAR defined name.
 *
 * Step 4:
 *	?????
 *
 * Step 5:
 *	Profit.
 *
 **/

typedef struct pcvar_s {
	char const * const name;
	char const * const defval;
	int const flags;
	qboolean const announce; //For Game and BGame.
	void (*update) (void);
} pcvar_t;

typedef struct vmPcvarfunc_s {
	char const * const name;
	void (*update) (void);
} vmPcvarfunc_t;

//==================
//GAME
//==================
#define PJK_GAME_INFINITE_FORCE_CVAR				"g_pjk_infforce"
#define PJK_GAME_INFINITE_FORCE_DVAL				"0"
#define PJK_GAME_INFINITE_FORCE_FLAG				CVAR_SERVERINFO

#define PJK_GAME_THERMAL_GOLF_CVAR					"g_pjk_thermalgolf"
#define PJK_GAME_THERMAL_GOLF_DVAL					"0"
#define PJK_GAME_THERMAL_GOLF_FLAG					CVAR_SERVERINFO

#define PJK_GAME_DISRUPTOR_INSTAKILL_CVAR			"g_pjk_ikdisrupt"
#define PJK_GAME_DISRUPTOR_INSTAKILL_DVAL			"0"
#define PJK_GAME_DISRUPTOR_INSTAKILL_FLAG			CVAR_SERVERINFO

#define PJK_GAME_DETPACK_LAUNCH_CVAR				"g_pjk_detlaunch"
#define PJK_GAME_DETPACK_LAUNCH_DVAL				"0"
#define PJK_GAME_DETPACK_LAUNCH_FLAG				CVAR_SERVERINFO

#define PJK_GAME_MASSMOD_CVAR						"g_pjk_massmod"
#define PJK_GAME_MASSMOD_DVAL						"1.0"
#define PJK_GAME_MASSMOD_FLAG						CVAR_SERVERINFO

#define PJK_GAME_ROCKETVEL_CVAR						"g_pjk_rocketvel"
#define PJK_GAME_ROCKETVEL_DVAL						"900.0"
#define PJK_GAME_ROCKETVEL_FLAG						CVAR_SERVERINFO

#define PJK_GAME_ROCKETSIZE_CVAR					"noni_g_pjk_rocketsize"
#define PJK_GAME_ROCKETSIZE_DVAL					"3.0"
#define PJK_GAME_ROCKETSIZE_FLAG					CVAR_SERVERINFO

#define PJK_GAME_ROCKETALTVELMOD_CVAR				"noni_g_pjk_rocketaltvelmod"
#define PJK_GAME_ROCKETALTVELMOD_DVAL				"0.5"
#define PJK_GAME_ROCKETALTVELMOD_FLAG				CVAR_SERVERINFO

extern const pcvar_t pjk_g_cvars[];
extern const size_t pjk_g_num;

//==================
//CLIENT GAME
//==================

extern const pcvar_t pjk_cg_cvars[];
extern const size_t pjk_cg_num;

/*
================================================================================================================================================
BOTH CLIENT AND SERVER GAME

	Cvars in here are kept the same both client and serverside.
	This means the Cvar affects something in Game AND CGame.

	NOTE:	Most (all) of these will require the CVAR_SYSTEMINFO flag.
			When the client leaves the server, they will take ALL of these cvar values with them.
			This isn't really a big concern, as most the server they next connect to will force its own settings on connect.
			However, if they were to start a solo game, it would mirror the settings of their last connected server.
			Again, not a huge deal, but users of ParaJK should be encouraged to keep a .cfg file of all their preferred settings.
================================================================================================================================================
*/

#define PJK_BGAME_FALLDAMAGE_CVAR					"bg_pjk_falldamage"
#define PJK_BGAME_FALLDAMAGE_DVAL					"1"
#define PJK_BGAME_FALLDAMAGE_FLAG					CVAR_SYSTEMINFO

#define PJK_BGAME_INFINITE_JUMP_CVAR				"bg_pjk_infjump"
#define PJK_BGAME_INFINITE_JUMP_DVAL				"0"
#define PJK_BGAME_INFINITE_JUMP_FLAG				CVAR_SYSTEMINFO

#define PJK_BGAME_SABER_LENGTH_CVAR					"bg_pjk_sblenmod"
#define PJK_BGAME_SABER_LENGTH_DVAL					"1.0"
#define PJK_BGAME_SABER_LENGTH_FLAG					CVAR_SYSTEMINFO

#define PJK_BGAME_JETPACK_FUEL_CVAR					"bg_pjk_jetfuel"
#define PJK_BGAME_JETPACK_FUEL_DVAL					"100.0"
#define PJK_BGAME_JETPACK_FUEL_FLAG					CVAR_SYSTEMINFO

extern const pcvar_t pjk_bg_cvars[];
extern const size_t pjk_bg_num;

//==================
//SERVER NON-GAME
//==================

#define PJK_SERV_SENDTARGETS_CVAR					"svp_sendtargets"
#define PJK_SERV_SENDTARGETS_DVAL					"1"
#define PJK_SERV_SENDTARGETS_FLAG					CVAR_ARCHIVE

extern const pcvar_t pjk_sv_cvars[];
extern const size_t pjk_sv_num;

#ifdef __cplusplus
}
#endif

#endif //Q_PARA_H
