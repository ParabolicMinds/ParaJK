#include "bg_para.h"

pbgcvar_t * pbgcvars;

static char const * bgnull = "";

char const * pjkBGCvarStrValue(const char *name) {
	if (pbgcvars) {
		pbgcvar_t const * pbgc;
		size_t i;
		for (i = 0, pbgc = pbgcvars; i < pjk_bg_num; i++, pbgc++) {
			if (!strcmp(name, pbgc->name)) {
				return pbgc->value;
			}
		}
		return bgnull;
	} else {
		return bgnull;
	}
}

int pjkBGCvarIntValue(const char *name) {
	return atoi(pjkBGCvarStrValue(name));
}

float pjkBGCvarFloatValue(const char *name) {
	return strtof(pjkBGCvarStrValue(name), NULL);
}
