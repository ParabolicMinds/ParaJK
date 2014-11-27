#ifndef BG_PARA_H
#define BG_PARA_H

#include "qcommon/q_shared.h"
#include "pcommon/q_para.h"

typedef struct pbgcvar_s {
	char const * name;
	char * value;
} pbgcvar_t;

extern pbgcvar_t * pbgcvars;

char const * pjkBGCvarStrValue(char const * name);
int pjkBGCvarIntValue(char const * name);
float pjkBGCvarFloatValue(char const * name);

#endif //BG_PARA_H
