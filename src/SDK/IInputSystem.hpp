#ifndef SDK_IINPUTSYSTEM_HPP
#define SDK_IINPUTSYSTEM_HPP

#include "ButtonCode.hpp"
#include "IAppSystem.hpp"

MAKE_CENUM_Q(InputStandardCursor, u32,
	NONE, 0,
	ARROW, 1,
	IBEAM, 2,
	HOURGLASS, 3,
	CROSSHAIR, 4,
	WAITARROW, 5,
	UP, 6,
	SIZE_NW_SE, 7,
	SIZE_NE_SW, 8,
	SIZE_W_E, 9,
	SIZE_N_S, 10,
	SIZE_ALL, 11,
	NO, 12,
	HAND, 13,
);

struct PlatWindow; // PlatWindow_t
struct InputCursorHandle; // InputCursorHandle_t

#define INVALID_USER_ID -1

class IInputSystem : public IAppSystem
{
public:
	/// Attach, detach input system from a particular window
	/// This window should be the root window for the application
	/// Only 1 window should be attached at any given time.
	virtual void AttachToWindow(void* hWnd) = 0;
	virtual void DetachFromWindow() = 0;

	/// Enables/disables input. PollInputState will not update current
	/// button/analog states when it is called if the system is disabled.
	virtual void EnableInput(bool enable) = 0;

	/// Enables/disables the windows message pump. PollInputState will not
	/// Peek/Dispatch messages if this is disabled
	virtual void EnableMessagePump(bool enable) = 0;

	/// Polls the current input state
	virtual void PollInputState() = 0;

	/// Gets the time of the last polling in ms
	virtual i32 GetPollTick() const = 0;

	/// Is a button down? "Buttons" are binary-state input devices (mouse buttons, keyboard keys)
	virtual bool IsButtonDown(ButtonCode code) const = 0;

	/// Returns the tick at which the button was pressed and released
	virtual i32 GetButtonPressedTick(ButtonCode code) const = 0;
	virtual i32 GetButtonReleasedTick(ButtonCode code) const = 0;

	/// Gets the value of an analog input device this frame
	/// Includes joysticks, mousewheel, mouse
	virtual i32 GetAnalogValue(AnalogCode code) const = 0;

	/// Gets the change in a particular analog input device this frame
	/// Includes joysticks, mousewheel, mouse
	virtual i32 GetAnalogDelta(AnalogCode code) const = 0;

	/// Returns the input events since the last poll
	virtual i32 GetEventCount() const = 0;
	virtual const InputEvent* GetEventData() const = 0;

	/// Posts a user-defined event into the event queue; this is expected
	/// to be called in overridden wndprocs connected to the root panel.
	virtual void PostUserEvent(const InputEvent& event) = 0;

	/// Returns the number of joysticks
	virtual i32 GetJoystickCount() const = 0;

	/// Enable/disable joystick, it has perf costs
	virtual void EnableJoystickInput(i32 joystick, bool enable) = 0;

	/// Enable/disable diagonal joystick POV (simultaneous POV buttons down)
	virtual void EnableJoystickDiagonalPOV(i32 joystick, bool enable) = 0;

	/// Sample the joystick and append events to the input queue
	virtual void SampleDevices() = 0;

	// FIXME: Currently force-feedback is only supported on the Xbox 360
	virtual void SetRumble(f32 leftMotor, f32 rightMotor, i32 userId = INVALID_USER_ID) = 0;
	virtual void StopRumble(i32 userId = INVALID_USER_ID) = 0;

	/// Resets the input state
	virtual void ResetInputState() = 0;

	/// Sets a player as the primary user - all other controllers will be ignored.
	virtual void SetPrimaryUserId(i32 userId) = 0;

	/// Convert back + forth between ButtonCode/AnalogCode + strings
	virtual const char* ButtonCodeToString(ButtonCode code) const = 0;
	virtual const char* AnalogCodeToString(AnalogCode code) const = 0;
	virtual ButtonCode StringToButtonCode(const char* string) const = 0;
	virtual AnalogCode StringToAnalogCode(const char* string) const = 0;

	/// Sleeps until input happens. Pass a negative number to sleep infinitely
	virtual void SleepUntilInput(i32 maxSleepTimeMS = -1) = 0;

	/// Convert back + forth between virtual codes + button codes
	// FIXME: This is a temporary piece of code
	virtual ButtonCode VirtualKeyToButtonCode(i32 virtualKey) const = 0;
	virtual i32 ButtonCodeToVirtualKey(ButtonCode code) const = 0;
	virtual ButtonCode ScanCodeToButtonCode(i32 param) const = 0;

	/// How many times have we called PollInputState?
	virtual i32 GetPollCount() const = 0;

	/// Sets the cursor position
	virtual void SetCursorPosition(i32 x, i32 y) = 0;

	/// Tells the input system to generate UI-related events, defined
	/// in inputsystem/inputenums.h (see IE_FirstUIEvent)
	/// We could have multiple clients that care about UI-related events
	/// so we refcount the clients with an Add/Remove strategy. If there
	/// are no interested clients, the UI events are not generated
	virtual void AddUIEventListener() = 0;
	virtual void RemoveUIEventListener() = 0;

	/// Returns the currently attached window
	virtual PlatWindow GetAttachedWindow() const = 0;

	/// Creates a cursor using one of the well-known cursor icons
	virtual InputCursorHandle GetStandardCursor(InputStandardCursor id) = 0;

	/// Loads a cursor defined in a file
	virtual InputCursorHandle LoadCursorFromFile(const char* fileName, const char* pathID = NULL) = 0;

	/// Sets the cursor icon
	virtual void SetCursorIcon(InputCursorHandle hCursor) = 0;

	/// Gets the cursor position
	virtual void GetCursorPosition(i32* x, i32* y) = 0;

	/// Mouse capture
	virtual void EnableMouseCapture(PlatWindow hWnd) = 0;
	virtual void DisableMouseCapture() = 0;
};

#endif // SDK_IINPUTSYSTEM_HPP
