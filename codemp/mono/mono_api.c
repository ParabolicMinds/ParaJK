#include "mono_api.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/image.h"

static MonoDomain * homeDomain;

monoapihandle_t * MonoAPI_Initialize(const char * assemblyFileName) {
	if (!homeDomain) {
		homeDomain = mono_jit_init("PJKSE");
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

char * MonoAPI_GetNewCharsFromString(mono_string * str) {
	if (!str) return NULL;
	return mono_string_to_utf8(str);
}

void MonoAPI_FreeMonoObject(void * mono_obj) {
	if (!mono_obj) return;
	mono_free(mono_obj);
}

monoImport_t * MonoAPI_CreateVMImport() {
	monoImport_t * mi;
	mi = calloc(1, sizeof(monoImport_t));
	mi->GetClassData = MonoAPI_GetClassData;
	mi->GetMethodPtr = MonoAPI_GetMethodPtr;
	mi->Initialize = MonoAPI_Initialize;
	mi->RegisterCMethod = MonoAPI_RegisterCMethod;
	mi->GetStaticMethod = MonoAPI_GetStaticMethod;
	mi->InvokeStaticMethod = MonoAPI_InvokeStaticMethod;
	mi->CharPtrToString = MonoAPI_CharPtrToString;
	mi->GetNewCharsFromString = MonoAPI_GetNewCharsFromString;
	mi->FreeMonoObject = MonoAPI_FreeMonoObject;
	return mi;
}
