// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "Effects.h"
#include "GeometricPrimitive.h"
#include "Model.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"


#include <dxerr.h>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <windowsx.h>
#include <Uxtheme.h>
#include <dwmapi.h>
#include <D3DX10math.h>
#include <d3d9types.h>
#include <d3d11.h>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include <directxmath.h>
#define NOMINMAX
#pragma comment(lib,"DirectXTK.lib")
#pragma comment(lib, "d3d11.lib")

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "DXErr.lib")
#endif

#pragma comment(lib, "dwmapi.lib")
#include <iomanip> //for time
#include <iostream>