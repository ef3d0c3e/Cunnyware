#ifndef SDK_ILAUNCHERMGR_HPP
#define SDK_ILAUNCHERMGR_HPP

#include "IAppSystem.hpp"

class CCocoaEvent;
class CStackCrawlParams;
struct SDL_Cursor;

class ILauncherMgr : public IAppSystem
{
public:
	virtual bool Connect(CreateInterfaceFn factory) = 0;
	virtual void Disconnect() = 0;

	virtual void* QueryInterface(const char* interfaceName) = 0;

	// Init, shutdown
	virtual InitReturnVal Init() = 0;
	virtual void Shutdown() = 0;

	// Create the window.
	virtual bool CreateGameWindow(const char* title, bool windowed, i32 width, i32 height) = 0;

	virtual void IncWindowRefCount() = 0;
	virtual void DecWindowRefCount() = 0;

	// Get the next N events. The function returns the number of events that were filled into your array.
	virtual i32 GetEvents(CCocoaEvent* pEvents, i32 maxEventsToReturn, bool debugEvents = false) = 0;
	virtual i32 PeekAndRemoveKeyboardEvents(bool* esc, bool* ret, bool* space, bool debugEvents = false) = 0;

	// Set the mouse cursor position.
	virtual void SetCursorPosition(i32 x, i32 y) = 0;

	virtual void SetWindowFullScreen(bool fullScreen, i32 width, i32 height) = 0;
	virtual bool IsWindowFullScreen() = 0;
	virtual void MoveWindow(i32 x, i32 y) = 0;
	virtual void SizeWindow(i32 width, i32 tall) = 0;
	virtual void PumpWindowsMessageLoop() = 0;

	virtual void DestroyGameWindow() = 0;
	virtual void SetApplicationIcon(const char* appIconFile) = 0;

	virtual void GetMouseDelta(i32& x, i32& y, bool ignoreNextMouseDelta = false) = 0;

	virtual void GetNativeDisplayInfo(i32 display, u32& width, u32& height, u32& refreshHz) = 0; // Retrieve the size of the monitor (desktop)
	virtual void RenderedSize(u32& width, u32& height, bool set) = 0; // either set or retrieve rendered size value (from dxabstract)
	virtual void DisplayedSize(u32& width, u32& height) = 0; // query backbuffer size (window size whether FS or windowed)

#if defined(DX_TO_GL_ABSTRACTION)
	virtual PseudoGLContextPtr GetMainContext() = 0;
	// Get the NSGLContext for a window's main view - note this is the carbon windowref as an argument
	virtual PseudoGLContextPtr GetGLContextForWindow(void* windowref) = 0;
	virtual PseudoGLContextPtr CreateExtraContext() = 0;
	virtual void DeleteContext(PseudoGLContextPtr hContext) = 0;
	virtual bool MakeContextCurrent(PseudoGLContextPtr hContext) = 0;
	virtual GLMDisplayDB* GetDisplayDB(void) = 0;
	virtual void GetDesiredPixelFormatAttribsAndRendererInfo(uint** ptrOut, uint* countOut, GLMRendererInfoFields* rendInfoOut) = 0;
	virtual void ShowPixels(CShowPixelsParams* params) = 0;
#endif

	virtual void GetStackCrawl(CStackCrawlParams* params) = 0;

	virtual void WaitUntilUserInput(i32 msSleepTime) = 0;

	virtual void* GetWindowRef() = 0;

	virtual void SetMouseVisible(bool state) = 0;
	virtual void SetMouseCursor(SDL_Cursor* hursor) = 0;
	virtual void SetForbidMouseGrab(bool forbidMouseGrab) = 0;
	virtual void OnFrameRendered() = 0;

	virtual void SetGammaRamp(const u16* red, const u16* green, const u16* blue) = 0;

	virtual double GetPrevGLSwapWindowTime() = 0;
};


#endif // SDK_ILAUNCHERMGR_HPP
