#pragma once

#include "RuntimePrefix.h"

#if LIGHTNING_ENGINE_EDITOR || LIGHTNING_ENGINE_PLATFORM_WIN
#define GLEW_STATIC
#include "External/Glew/include/GL/glew.h"
#include "External/Glew/include/GL/wglew.h"
#elif LIGHTNING_ENGINE_PLATFORM_ANDROID

#elif LIGHTNING_ENGINE_IOS
#endif
