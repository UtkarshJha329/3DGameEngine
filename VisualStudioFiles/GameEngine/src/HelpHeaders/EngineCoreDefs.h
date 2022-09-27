#pragma once

#ifdef SORA_PLATFORM_WINDOWS
#ifdef SORA_BUILD_DLL
#define SORA_API __declspec(dllexport)
#else
#define SORA_API __declspec(dllimport)
#endif
#else
#error Sora Engine currently only supports windows!
#endif
