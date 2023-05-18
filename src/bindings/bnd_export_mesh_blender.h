#include "bindings.h"

#pragma once

// A quick implementation to export a list of mesh to blender.

#if defined(ON_PYTHON_COMPILE)
void initBlenderExporterBindings(pybind11::module& m);
#else
void initBlenderExporterBindings(void* m);
#endif