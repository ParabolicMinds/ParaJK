#include "qcommon/q_shared.h"
#include "pcommon/q_para.h"
#include "g_local.h"
#include "bg_para.h"

int pjkGCvarIntValue(const char *name);
float pjkGCvarFloatValue(const char *name);
char * pjkGCvarStringValue(const char * name);

extern vmPcvarfunc_t const vmCvarGFuncs[];
extern int const vmCvarGFuncsLen;

typedef struct gGolfBall_s {
	int RGB[3]; //0 - 255
	gentity_t * ent;
} gGolfBall_t;

extern gGolfBall_t par_golfBalls[];

void G_PJK_UpdateSaberLengths ();
void G_PJK_UpdateJetpackTanks();
void G_PJK_UpdateThermalGolfMode();

extern int shaderRemapsThisFrame;
