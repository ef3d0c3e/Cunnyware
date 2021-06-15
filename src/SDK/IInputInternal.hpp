#ifndef SDK_IINPUTINTERNAL_HPP
#define SDK_IINPUTINTERNAL_HPP

#include "IInput.hpp"

typedef i32 HInputContext;

class IInputInternal : public IInput
{
public:
	// processes input for a frame
	virtual void RunFrame() = 0;

	virtual void UpdateMouseFocus(i32 x, i32 y) = 0;

	// called when a panel becomes invalid
	virtual void PanelDeleted(VPANEL panel) = 0;

	// inputs into vgui input handling
	virtual bool InternalCursorMoved(i32 x, i32 y) = 0; //expects input in surface space
	virtual bool InternalMousePressed(MouseCode code) = 0;
	virtual bool InternalMouseDoublePressed(MouseCode code) = 0;
	virtual bool InternalMouseReleased(MouseCode code) = 0;
	virtual bool InternalMouseWheeled(i32 delta) = 0;
	virtual bool InternalKeyCodePressed(KeyCode code) = 0;
	virtual void InternalKeyCodeTyped(KeyCode code) = 0;
	virtual void InternalKeyTyped(wchar_t unichar) = 0;
	virtual bool InternalKeyCodeReleased(KeyCode code) = 0;

	// Creates/ destroys "input" contexts, which contains information
	// about which controls have mouse + key focus, for example.
	virtual HInputContext CreateInputContext() = 0;
	virtual void DestroyInputContext(HInputContext context) = 0;

	// Associates a particular panel with an input context
	// Associating NULL is valid; it disconnects the panel from the context
	virtual void AssociatePanelWithInputContext(HInputContext context, VPANEL root) = 0;

	// Activates a particular input context, use DEFAULT_INPUT_CONTEXT
	// to get the one normally used by VGUI
	virtual void ActivateInputContext(HInputContext context) = 0;

	// This method is called to post a cursor message to the current input context
	virtual void PostCursorMessage() = 0;

	// Cursor position; this is the current position read from the input queue.
	// We need to set it because client code may read this during Mouse Pressed
	// events, etc.
	virtual void UpdateCursorPosInternal(i32 x, i32 y) = 0;

	// Called to handle explicit calls to CursorSetPos after input processing is complete
	virtual void HandleExplicitSetCursor() = 0;

	// Updates the internal key/mouse state associated with the current input context without sending messages
	virtual void SetKeyCodeState(KeyCode code, bool bPressed) = 0;
	virtual void SetMouseCodeState(MouseCode code, MouseCodeState state) = 0;
	virtual void UpdateButtonState(const InputEvent& event) = 0;

	// Resets a particular input context, use DEFAULT_INPUT_CONTEXT
	// to get the one normally used by VGUI
	virtual void ResetInputContext(HInputContext context) = 0;
};

#endif // SDK_IINPUTINTERNAL_HPP
