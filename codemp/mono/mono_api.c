#include "mono_api.h"
#include "mono_backend.h"

int (*Test) (void);

void MONO_API_Initialize() {
	MONO_Initialize();
	Test = mono_method_get_unmanaged_thunk(testMethod);
}

int MONO_API_IsReady() {
	if (testMethod) return 1;
	return 0;
}
