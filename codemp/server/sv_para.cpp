#include "qcommon/qcommon.h"
#include "sv_para.h"


//ALL CVARS are setup here to make them immedaitely visible.
void SV_PARA_INIT() {
	pcvar_t const * cv;
	size_t cvn;
	for (cvn = 0, cv = &pjk_g_cvars[cvn]; cvn < pjk_g_num; cvn++, cv = &pjk_g_cvars[cvn]) {
		Cvar_Get(cv->name, cv->defval, cv->cvarFlags);
	}
	for (cvn = 0, cv = &pjk_cg_cvars[cvn]; cvn < pjk_cg_num; cvn++, cv = &pjk_cg_cvars[cvn]) {
		Cvar_Get(cv->name, cv->defval, cv->cvarFlags);
	}
	for (cvn = 0, cv = &pjk_bg_cvars[cvn]; cvn < pjk_bg_num; cvn++, cv = &pjk_bg_cvars[cvn]) {
		Cvar_Get(cv->name, cv->defval, cv->cvarFlags);
	}
	for (cvn = 0, cv = &pjk_sv_cvars[cvn]; cvn < pjk_sv_num; cvn++, cv = &pjk_sv_cvars[cvn]) {
		Cvar_Get(cv->name, cv->defval, cv->cvarFlags);
	}
}
