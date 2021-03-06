// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <vector>

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif


#ifdef NDEBUG
#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#define DxAssert(x)		{ if (FAILED(x)) { MessageBox(NULL, _T("DxAssert"), _T("ERROR"), MB_OK); } }
#else
#define DxAssert(x) { x }
#endif
#else
#include <assert.h>
#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#define DxAssert(x)		{ if (FAILED(x)) { MessageBox(NULL, _T("DxAssert"), _T("ERROR"), MB_OK); assert(SUCCEEDED(x)); } }
#else
#define DxAssert(x)		{ assert(SUCCEEDED(x)) }
#endif
#endif
// TODO: reference additional headers your program requires here
