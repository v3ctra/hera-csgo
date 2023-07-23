#pragma once

#ifdef _MSC_VER
#define ALWAYS_INLINE __forceinline
#else
#define ALWAYS_INLINE __attribute__ ( ( always_inline ) ) inline
#endif

#if defined( _WIN32 ) || defined( _WIN64 )
#define NOMINMAX
#include <windows.h>
#endif

#include <algorithm>
#include <fstream>
#include <numbers>
#include <cstddef>
#include <thread>
#include <string>
#include <vector>
#include <memory>

//---thirdparty---
#include <x86retspoof.hpp>
#include <json/json.hpp>
#include <hash.hpp>
#include <xor.hpp>

#include "winapi/winapi.hpp"
#include "pe/pe.hpp"

#include "memory/memory.hpp"

#ifdef _DEBUG
#include <iostream>
#define LOG(x) std::cout<<"$ [debug] "<< x <<std::endl;
#else
#define LOG(x)
#endif