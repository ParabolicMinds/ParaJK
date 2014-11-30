#ifndef MONO_API_H
#define MONO_API_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mono_api_vm.h"

/*
================================
ParaJK Mono API

TODO: Not leak memory when things fail.
	Temporary workaround: Don't fail.
	(It's not like we leak a huge amount of memory... I think...)
================================
*/

monoapidomain_t *	MonoAPI_Initialize(char * appDomainName, char const * assemblyFileName); //Returns a handle to the initialized domain. you need to keep this until shutdown. Returns NULL on failure.
qboolean			MonoAPI_SetDomainActive(monoapidomain_t * mapi); //May fail, be sure to handle it.

mono_class *		MonoAPI_GetClassData(monoapidomain_t * mapi, char const * _namespace, char const * name);
void *				MonoAPI_GetMethodPtr(mono_class * _class, char const * method_name, int param_count);

monoImport_t *		MonoAPI_CreateVMImport();

#ifdef __cplusplus
}
#endif

#endif //MONO_API_H
