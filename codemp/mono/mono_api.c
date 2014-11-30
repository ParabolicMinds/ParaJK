#include "mono_api.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/image.h"

static MonoDomain * homeDomain;

monoapidomain_t * MonoAPI_Initialize(char * appDomainName, const char * assemblyFileName) {
	if (!homeDomain) {
		homeDomain = mono_jit_init("PJKSE");
		if (!homeDomain)
			return NULL;
	}

	monoapidomain_t * mapi = malloc(sizeof(monoapidomain_t));
	mapi->domainHandle = mono_domain_create_appdomain(appDomainName, NULL);
	if (!mapi->domainHandle) {
		MonoAPI_ShutdownDomain(mapi);
		return NULL;
	}
	mapi->assemblyHandle = mono_domain_assembly_open(mapi->domainHandle, assemblyFileName);
	if (!mapi->assemblyHandle) {
		MonoAPI_ShutdownDomain(mapi);
		return NULL;
	}
	mapi->imageHandle = mono_assembly_get_image(mapi->assemblyHandle);
	if (!mapi->imageHandle) {
		MonoAPI_ShutdownDomain(mapi);
		return NULL;
	}
	return mapi;
}

qboolean MonoAPI_SetDomainActive(monoapidomain_t * mapi) {
	return mono_domain_set(mapi->domainHandle, 0);
}

void MonoAPI_ShutdownDomain(monoapidomain_t * mapi) {
	if (mapi->domainHandle)
		mono_domain_unload(mapi->domainHandle);
	if (mapi->assemblyHandle)
		mono_assembly_close(mapi->assemblyHandle);
	if (mapi->imageHandle)
		mono_image_close(mapi->imageHandle);
	free(mapi);
}

void MonoAPI_ShutdownAPI() {
	mono_jit_cleanup(homeDomain);
	homeDomain = NULL;
}

mono_class * MonoAPI_GetClassData(monoapidomain_t * mapi, char const * _namespace, char const * name) {
	return (void *) mono_class_from_name(mapi->imageHandle, _namespace, name);
}

void * MonoAPI_GetMethodPtr(mono_class * _class, char const * method_name, int param_count) {
	MonoMethod * mt = mono_class_get_method_from_name(_class, method_name, param_count);
	if (!mt) return NULL;
	void * retval = mono_method_get_unmanaged_thunk(mt);
	mono_free_method(mt);
	return retval;
}

monoImport_t * MonoAPI_CreateVMImport() {
	monoImport_t * mi;
	mi = calloc(1, sizeof(monoImport_t));
	mi->MonoAPI_GetClassData = MonoAPI_GetClassData;
	mi->MonoAPI_GetMethodPtr = MonoAPI_GetMethodPtr;
	mi->MonoAPI_Initialize = MonoAPI_Initialize;
	mi->MonoAPI_SetDomainActive = MonoAPI_SetDomainActive;
	mi->MonoAPI_ShutdownDomain = MonoAPI_ShutdownDomain;
	return mi;
}
