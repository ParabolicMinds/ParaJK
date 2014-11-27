#ifndef Q_PARASTATE_CLSV_H
#define Q_PARASTATE_CLSV_H

#include "q_parastate.h"
#include "qcommon/qcommon.h"

void	CLSV_InitializeParaStates();
void	CLSV_CleanupParaStates();
void	CLSV_SendParaState(msg_t * buf, int clientNum);
int		CLSV_ParseParaState(msg_t * buf);
void	CLSV_PGameInit(msg_t * buf);

extern paraState_t CLSV_ParaStates[];
extern paraState_t CL_ParaState;

#endif
