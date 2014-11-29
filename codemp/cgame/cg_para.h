#ifndef CG_PARA_H
#define CG_PARA_H

#include "qcommon/q_shared.h"
#include "pcommon/q_para.h"
#include "cg_local.h"
#include "game/bg_para.h"

int pjkCGCvarIntValue(const char *name);
float pjkCGCvarFloatValue(const char *name);
char * pjkCGCvarStringValue(const char * name);

extern vmPcvarfunc_t const vmCvarCGFuncs[];
extern int const vmCvarCGFuncsLen;

void CG_PJK_UpdateSaberLengths ();

#endif
