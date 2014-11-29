#include "mono_backend.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/image.h"

static MonoDomain *domain;
static MonoAssembly *assembly;
static MonoImage *image;
static MonoClass *pjkse;

MonoMethod * testMethod;

void MONO_Initialize() {
	domain = mono_jit_init ("PJKSE");
	assembly = mono_domain_assembly_open (domain, "para/pjkse.dll");
	if (!assembly) {
	  //TODO: Do something errorish
	}
	image = mono_assembly_get_image(assembly);
	if (!image) {
	  //TODO: Do something errorish
	}
	pjkse = mono_class_from_name(image, "PJKSE", "PJKSE_STATIC");
	if (!pjkse) {
	  //TODO: Do something errorish
	}

	testMethod = mono_class_get_method_from_name(pjkse, "Test", 0);
}

void MONO_Shutdown() {
	//TODO: Cleanup
}
