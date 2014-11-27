#include "qcommon/qcommon.h"
#include "sv_para.hpp"


//ALL CVARS are setup here to make them immedaitely visible.
void SV_PARA_INIT() {
	pcvar_t const * cv;
	size_t cvn;
	for (cvn = 0, cv = &pjk_g_cvars[cvn]; cvn < pjk_g_num; cvn++, cv = &pjk_g_cvars[cvn]) {
		Cvar_Get(cv->name, cv->defval, cv->flags);
	}
	for (cvn = 0, cv = &pjk_cg_cvars[cvn]; cvn < pjk_cg_num; cvn++, cv = &pjk_cg_cvars[cvn]) {
		Cvar_Get(cv->name, cv->defval, cv->flags);
	}
	for (cvn = 0, cv = &pjk_bg_cvars[cvn]; cvn < pjk_bg_num; cvn++, cv = &pjk_bg_cvars[cvn]) {
		Cvar_Get(cv->name, cv->defval, cv->flags);
	}
	for (cvn = 0, cv = &pjk_sv_cvars[cvn]; cvn < pjk_sv_num; cvn++, cv = &pjk_sv_cvars[cvn]) {
		Cvar_Get(cv->name, cv->defval, cv->flags);
	}
}

int pjkCvarIntValue(const char * name) {
	return Cvar_VariableIntegerValue(name);
}

float pjkCvarFloatValue(const char * name) {
	return Cvar_VariableValue(name);
}

std::string pjkCvarStrValue(const char * name) {
	return std::string(Cvar_VariableString(name));
}
