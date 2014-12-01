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

//Returns a handle to the initialized domain. you need to keep this until shutdown. Returns NULL on failure.
monoapihandle_t *	MonoAPI_Initialize(char const * assemblyFileName);

//Gets a typedef'd void pointer to a MonoClass instance.
mono_class *		MonoAPI_GetClassData(monoapihandle_t * mapi, char const * _namespace, char const * name);

//Appears to cause segfaults if the function deals with pointers, use GetStaticMethod and InvokeStaticMethod instead.
void *				MonoAPI_GetMethodPtr(mono_class * _class, char const * method_name, int param_count);

//Gets a typedef'd void pointer to a static MonoMethod instance.
mono_method *		MonoAPI_GetStaticMethod(mono_class * _class, char const * method_name, int param_count);

//Invokation call for mono_methods returned in the previous function.
void *				MonoAPI_InvokeStaticMethod(mono_method * method, void ** params, char ** exception);

void				MonoAPI_RegisterCMethod(char const * internalMethod, void const * cFunc);
mono_string *		MonoAPI_CharPtrToString(char const * data);
char *				MonoAPI_GetNewCharsFromString(mono_string * str);

//To put it simply, if ANY of the functions in this header return a pointer, there needs to be one of these further down the line.
//Exception 1: MonoAPI_Initialize's "monoapihandle_t", there will be a shutdown method for these in the future. DO NOT PASS THEM TO THIS FUNCTION.
//Exception 2: MonoAPI_CreateVMImport's "monoImport_t", just call regular free() on this.
void				MonoAPI_FreeMonoObject(void * mono_obj);

monoImport_t *		MonoAPI_CreateVMImport();

#ifdef __cplusplus
}
#endif

#endif //MONO_API_H
