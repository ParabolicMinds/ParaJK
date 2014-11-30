#ifndef MONO_API_VM_H
#define MONO_API_VM_H

#include "qcommon/q_shared.h"

typedef void mono_class;

typedef struct monoapidomain_s {
	void * domainHandle;
	void * assemblyHandle;
	void * imageHandle;
} monoapidomain_t;

typedef struct monoImport_s {
	monoapidomain_t *		(*MonoAPI_Initialize) (char * domain_name, char const * file_name);
	qboolean				(*MonoAPI_SetDomainActive) (monoapidomain_t * mapi);
	void					(*MonoAPI_ShutdownDomain) (monoapidomain_t * mapi);
	mono_class *			(*MonoAPI_GetClassData) (monoapidomain_t * mapi, char const * _namespace, char const * name);
	void *					(*MonoAPI_GetMethodPtr) (mono_class * _class, char const * method_name, int param_count);
} monoImport_t;


#endif //MONO_API_VM_H
