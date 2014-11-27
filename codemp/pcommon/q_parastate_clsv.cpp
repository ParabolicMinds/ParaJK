#include "q_parastate_clsv.h"

void CLSV_InitializeParaStates() {
	paraState_t * ps;
	size_t i;

	CLSV_CleanupParaStates();
	for (i = 0, ps = CLSV_ParaStates; i < MAX_CLIENTS; i++, ps++) {
		ps->msg = new char[PJK_PARASTATE_MSG_SIZE]();
	}

	if (!CL_ParaState.msg) CL_ParaState.msg = new char[PJK_PARASTATE_MSG_SIZE]();
}

void CLSV_CleanupParaStates() {
	paraState_t * ps;
	size_t i;

	for (i = 0, ps = CLSV_ParaStates; i < MAX_CLIENTS; i++, ps++) {
		if (ps->msg) delete [] ps->msg;
	}

	memset(CLSV_ParaStates, 0x00, sizeof(paraState_t)*MAX_CLIENTS);
}

void CLSV_SendParaState(msg_t * buf, int clientNum) {
	MSG_WriteLong(buf, clientNum);
	MSG_WriteString(buf, CLSV_ParaStates[clientNum].msg);
}

int CLSV_ParseParaState(msg_t * buf) {
	int clientNum = MSG_ReadLong( buf );
	paraState_t * ps = CLSV_ParaStates + clientNum;
	memset(ps->msg, 0x00, PJK_PARASTATE_MSG_SIZE);
	strncpy(ps->msg, MSG_ReadString(buf), PJK_PARASTATE_MSG_SIZE);
	return clientNum;
}

void CLSV_PGameInit(msg_t * buf) {
	size_t i;
	paraState_t * ps;
	for (i = 0, ps = CLSV_ParaStates; i < MAX_CLIENTS; i++, ps++) {
		CLSV_ParseParaState(buf);
	}
}

paraState_t CLSV_ParaStates[MAX_CLIENTS];
paraState_t CL_ParaState;
