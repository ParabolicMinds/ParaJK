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

monoapihandle_t *	MonoAPI_Initialize(char const * assemblyFileName); //Returns a handle to the initialized domain. you need to keep this until shutdown. Returns NULL on failure.

mono_class *		MonoAPI_GetClassData(monoapihandle_t * mapi, char const * _namespace, char const * name);
void *				MonoAPI_GetMethodPtr(mono_class * _class, char const * method_name, int param_count); //Causes some Segfault problems... Dunno what the deal is, avoid for now.
mono_method *		MonoAPI_GetStaticMethod(mono_class * _class, char const * method_name, int param_count);
void *				MonoAPI_InvokeStaticMethod(mono_method * method, void ** params);
void				MonoAPI_RegisterCMethod(char const * internalMethod, void const * cFunc);
mono_string *		MonoAPI_CharPtrToString(char const * data);
char *				MonoAPI_GetNewCharsFromString(mono_string * str);
void				MonoAPI_FreeMonoObject(void * mono_obj);
monoImport_t *		MonoAPI_CreateVMImport();

#ifdef __cplusplus
}
#endif

#endif //MONO_API_H
