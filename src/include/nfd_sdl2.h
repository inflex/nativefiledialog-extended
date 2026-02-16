/*
  Native File Dialog Extended
  Repository: https://github.com/btzy/nativefiledialog-extended
  License: Zlib
  Authors: Bernard Teo

  This header contains a function to convert an SDL window handle to a native window handle for
  passing to NFDe.

  This is meant to be used with SDL2, but if there are incompatibilities with future SDL versions,
  we can conditionally compile based on SDL_MAJOR_VERSION.
 */

#ifndef _NFD_SDL2_H
#define _NFD_SDL2_H


#ifdef _MSC_VER
#include <SDL.h>
#include <SDL_error.h>
#elif defined __APPLE__
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#else
#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#endif


#include <nfd.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#define NFD_INLINE inline
#else
#define NFD_INLINE static inline
#endif  // __cplusplus

/**
 *  Converts an SDL window handle to a native window handle that can be passed to NFDe.
 *  @param sdlWindow The SDL window handle.
 *  @param[out] nativeWindow The output native window handle, populated if and only if this function
 *  returns true.
 *  @return Either true to indicate success, or false to indicate failure.  If false is returned,
 * you can call SDL_GetError() for more information.  However, it is intended that users ignore the
 * error and simply pass a value-initialized nfdwindowhandle_t to NFDe if this function fails. */
NFD_INLINE bool NFD_GetNativeWindowFromSDLWindow(SDL_Window* sdlWindow,
                                                 nfdwindowhandle_t* nativeWindow) {

#if defined(SDL_PLATFORM_WIN32)
    HWND hwnd = (HWND)SDL_GetPointerProperty(SDL_GetWindowProperties(sdlWindow), SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
    nativeWindow->type = NFD_WINDOW_HANDLE_TYPE_WINDOWS;
    nativeWindow->handle = (void*)hwnd;
        return true;
#elif defined(SDL_PLATFORM_MACOS)
    nativeWindow->handle = (void *)SDL_GetPointerProperty(SDL_GetWindowProperties(sdlWindow), SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, NULL);
    nativeWindow->type = NFD_WINDOW_HANDLE_TYPE_COCOA;
        return true;
#elif defined(SDL_PLATFORM_LINUX)
    if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "x11") == 0) {
        nativeWindow->handle = (void *)SDL_GetNumberProperty(SDL_GetWindowProperties(sdlWindow), SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0);
        nativeWindow->type = NFD_WINDOW_HANDLE_TYPE_X11;
        return true;

	} else if (SDL_strcmp(SDL_GetCurrentVideoDriver(), "wayland") == 0) {
        nativeWindow->handle = (void *)SDL_GetNumberProperty(SDL_GetWindowProperties(sdlWindow), SDL_PROP_WINDOW_WIN32_HWND_POINTER, 0);
        nativeWindow->type = NFD_WINDOW_HANDLE_TYPE_X11;
    }
#endif


	 /*
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (!SDL_GetWindowWMInfo(sdlWindow, &info)) {
        return false;
    }
    switch (info.subsystem) {
#if defined(SDL_VIDEO_DRIVER_WINDOWS)
        case SDL_SYSWM_WINDOWS:
            nativeWindow->type = NFD_WINDOW_HANDLE_TYPE_WINDOWS;
            nativeWindow->handle = (void*)info.info.win.window;
            return true;
#endif
#if defined(SDL_VIDEO_DRIVER_COCOA)
        case SDL_SYSWM_COCOA:
            nativeWindow->type = NFD_WINDOW_HANDLE_TYPE_COCOA;
            nativeWindow->handle = (void*)info.info.cocoa.window;
            return true;
#endif
#if defined(SDL_VIDEO_DRIVER_X11)
        case SDL_SYSWM_X11:
            nativeWindow->type = NFD_WINDOW_HANDLE_TYPE_X11;
            nativeWindow->handle = (void*)info.info.x11.window;
            return true;
#endif
        default:
            // Silence the warning in case we are not using a supported backend.
            (void)nativeWindow;
            SDL_SetError("Unsupported native window type.");
            return false;
    }
	 */
	 return false;
}

#undef NFD_INLINE
#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // _NFD_SDL2_H
