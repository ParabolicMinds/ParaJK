#ifndef MONO_API_VM_H
#define MONO_API_VM_H

#ifdef __mono_enable

#include "qcommon/q_shared.h"

typedef void mono_class, mono_method, mono_string, mono_object;

typedef struct monoapihandle_s {
	void * assemblyHandle;
	void * imageHandle;
} monoapihandle_t;

typedef struct monoImport_s {
	monoapihandle_t *		(*Initialize) (char const * file_name);
	void					(*ShutdownAPIHandle)(monoapihandle_t * apih);
	mono_class *			(*GetClassData) (monoapihandle_t * mapi, char const * _namespace, char const * name);
	void *					(*GetMethodPtr) (mono_class * _class, char const * method_name, int param_count);
	void					(*RegisterCMethod) (char const * internalMethod, void const * cFunc);
	mono_method *			(*GetStaticMethod)(mono_class * _class, char const * method_name, int param_count);
	void *					(*InvokeStaticMethod)(mono_method * method, void ** params, char ** exception);
	mono_string *			(*CharPtrToString)(char const * data);
	mono_string *			(*CharPtrToStringTemporary)(char const * data);
	char *					(*GetNewCharsFromString)(mono_string * str);
	void					(*FreeMonoObject)(void * mono_obj);
	void					(*FreeVMImport)(struct monoImport_s * import);
	void *					(*UnboxMonoObject)(mono_object * obj);
} monoImport_t;

#endif

#endif //MONO_API_VM_H
