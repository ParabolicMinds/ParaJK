#ifndef MONO_BACKEND_H
#define MONO_BACKEND_H

#include "mono/metadata/class.h"
#include "mono/metadata/object.h"

void MONO_Initialize();
void MONO_Shutdown();

//Functions
extern MonoMethod * testMethod;

#endif //MONO_BACKEND_H
