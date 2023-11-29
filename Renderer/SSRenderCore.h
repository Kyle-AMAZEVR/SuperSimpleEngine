

#pragma once
#   ifdef RENDERCORE_EXPORTS
#       define DX11RENDERER_API __declspec(dllexport)
#		define DX11RENDERER_API_EXTERN extern __declspec(dllexport)
#   elif defined(RENDERCORE_IMPORTS)
#       define DX11RENDERER_API __declspec(dllimport)
#		define DX11RENDERER_API_EXTERN extern __declspec(dllimport)
#   else
#	ifndef DX11RENDERER_API
#       define DX11RENDERER_API
#	endif
#   endif

#include <d3d11.h>
#include <memory>