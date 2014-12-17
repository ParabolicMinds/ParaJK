#ifdef __mono_enable

#include "mono_api.h"

#include "mono/jit/jit.h"
#include "mono/metadata/mono-config.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/image.h"

static MonoDomain * homeDomain;

monoapihandle_t * MonoAPI_Initialize(const char * assemblyFileName) {
	if (!homeDomain) {
		mono_config_parse (NULL);
		homeDomain = mono_jit_init_version("PJKSE", "v4.0");
		if (!homeDomain)
			return NULL;
	}

	monoapihandle_t * mapi = malloc(sizeof(monoapihandle_t));
	mapi->assemblyHandle = mono_domain_assembly_open(homeDomain, assemblyFileName);
	if (!mapi->assemblyHandle) {
		return NULL;
	}
	mapi->imageHandle = mono_assembly_get_image(mapi->assemblyHandle);
	if (!mapi->imageHandle) {
		return NULL;
	}
	return mapi;
}

void MonoAPI_ShutdownAPIHandle(monoapihandle_t * apih) {
	//TODO
}

mono_class * MonoAPI_GetClassData(monoapihandle_t * mapi, char const * _namespace, char const * name) {
	if (!mapi) return NULL;
	return mono_class_from_name(mapi->imageHandle, _namespace, name);
}

void * MonoAPI_GetMethodPtr(mono_class * _class, char const * method_name, int param_count) {
	MonoMethod * mt = mono_class_get_method_from_name(_class, method_name, param_count);
	if (!mt) return NULL;
	void * retval = mono_method_get_unmanaged_thunk(mt);
	mono_free_method(mt);
	return retval;
}

mono_method * MonoAPI_GetStaticMethod(mono_class * _class, char const * method_name, int param_count) {
	if (!_class) return NULL;
	return mono_class_get_method_from_name(_class, method_name, param_count);
}

void * MonoAPI_InvokeStaticMethod(mono_method * method, void ** params, char ** exception) {
	if (!method) return NULL;
	MonoObject * exc;
	MonoObject * retval = mono_runtime_invoke(method, NULL, params, &exc);
	if (exc) {
		if (exception)
			*exception = mono_string_to_utf8(mono_object_to_string(exc, NULL));
		mono_free(retval);
		return NULL;
	}
	void * retvalv;
	if (!mono_type_is_void(mono_signature_get_return_type(mono_method_signature(method)))) {
		retvalv = mono_object_unbox(retval);
	} else retvalv = NULL;
	mono_free(retval);
	return retvalv;
}

void MonoAPI_RegisterCMethod(char const * internalMethod, void const * cFunc) {
	mono_add_internal_call(internalMethod, cFunc);
}

mono_string * MonoAPI_CharPtrToString(char const * data) {
	return mono_string_new(homeDomain, data);

}

static MonoString * tempStr;
mono_string * MonoAPI_CharPtrToStringTemporary(char const * data) {
	if (tempStr) {
		mono_free(tempStr);
		tempStr = NULL;
	}
	tempStr = mono_string_new(homeDomain, data);
	return tempStr;

}

char * MonoAPI_GetNewCharsFromString(mono_string * str) {
	if (!str) return NULL;
	return mono_string_to_utf8(str);
}

void MonoAPI_FreeMonoObject(void * mono_obj) {
	if (!mono_obj) return;
	mono_free(mono_obj);
}

void * MonoAPI_UnboxMonoObject(mono_object * obj) {
	return mono_object_unbox(obj);
}

monoImport_t * MonoAPI_CreateVMImport() {
	monoImport_t * mi;
	mi = calloc(1, sizeof(monoImport_t));
	mi->GetClassData = MonoAPI_GetClassData;
	mi->GetMethodPtr = MonoAPI_GetMethodPtr;
	mi->Initialize = MonoAPI_Initialize;
	mi->ShutdownAPIHandle = MonoAPI_ShutdownAPIHandle;
	mi->RegisterCMethod = MonoAPI_RegisterCMethod;
	mi->GetStaticMethod = MonoAPI_GetStaticMethod;
	mi->InvokeStaticMethod = MonoAPI_InvokeStaticMethod;
	mi->CharPtrToString = MonoAPI_CharPtrToString;
	mi->CharPtrToStringTemporary = MonoAPI_CharPtrToStringTemporary;
	mi->GetNewCharsFromString = MonoAPI_GetNewCharsFromString;
	mi->FreeMonoObject = MonoAPI_FreeMonoObject;
	mi->FreeVMImport = MonoAPI_FreeVMImport;
	mi->UnboxMonoObject = MonoAPI_UnboxMonoObject;
	return mi;
}

void MonoAPI_FreeVMImport(monoImport_t * import) {
	if (import)
		free(import);
}

#endif
