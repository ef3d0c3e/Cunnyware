#ifndef SDK_IINPUT_HPP
#define SDK_IINPUT_HPP

#include "ButtonCode.hpp"
#include "IBaseInterface.hpp"

typedef u64 HCursor;
typedef u32 VPANEL;

class IInput : public IBaseInterface
{
public:
	virtual void SetMouseFocus(VPANEL newMouseFocus) = 0;
	virtual void SetMouseCapture(VPANEL panel) = 0;

	// returns the string name of a scan code
	virtual void GetKeyCodeText(KeyCode code, char* buf, i32 buflen) = 0;

	// focus
	virtual VPANEL GetFocus() = 0;
	virtual VPANEL GetCalculatedFocus() = 0; // to handle cases where the focus changes inside a frame.
	virtual VPANEL GetMouseOver() = 0; // returns the panel the mouse is currently over, ignoring mouse capture

	// mouse state
	virtual void SetCursorPos(i32 x, i32 y) = 0;
	virtual void GetCursorPos(i32& x, i32& y) = 0;
	virtual bool WasMousePressed(MouseCode code) = 0;
	virtual bool WasMouseDoublePressed(MouseCode code) = 0;
	virtual bool IsMouseDown(MouseCode code) = 0;

	// cursor override
	virtual void SetCursorOveride(HCursor cursor) = 0;
	virtual HCursor GetCursorOveride() = 0;

	// key state
	virtual bool WasMouseReleased(MouseCode code) = 0;
	virtual bool WasKeyPressed(KeyCode code) = 0;
	virtual bool IsKeyDown(KeyCode code) = 0;
	virtual bool WasKeyTyped(KeyCode code) = 0;
	virtual bool WasKeyReleased(KeyCode code) = 0;

	virtual VPANEL GetAppModalSurface() = 0;
	// set the modal dialog panel.
	// all events will go only to this panel and its children.
	virtual void SetAppModalSurface(VPANEL panel) = 0;
	// release the modal dialog panel
	// do this when your modal dialog finishes.
	virtual void ReleaseAppModalSurface() = 0;

	virtual void GetCursorPosition(i32& x, i32& y) = 0;

	virtual void SetIMEWindow(void* hwnd) = 0;
	virtual void* GetIMEWindow() = 0;

	virtual void OnChangeIME(bool forward) = 0;
	virtual i32 GetCurrentIMEHandle() = 0;
	virtual i32 GetEnglishIMEHandle() = 0;

	// Returns the Language Bar label (Chinese, Korean, Japanese, Russion, Thai, etc.)
	virtual void GetIMELanguageName(wchar_t* buf, i32 unicodeBufferSizeInBytes) = 0;
	// Returns the short code for the language (EN, CH, KO, JP, RU, TH, etc. ).
	virtual void GetIMELanguageShortCode(wchar_t* buf, i32 unicodeBufferSizeInBytes) = 0;

	struct LanguageItem
	{
		wchar_t shortname[4];
		wchar_t menuname[128];
		i32 handleValue;
		bool active; // true if this is the active language
	};

	struct ConversionModeItem
	{
		wchar_t menuname[128];
		i32 handleValue;
		bool active; // true if this is the active conversion mode
	};

	struct SentenceModeItem
	{
		wchar_t menuname[128];
		i32 handleValue;
		bool active; // true if this is the active sentence mode
	};

	// Call with NULL dest to get item count
	virtual i32 GetIMELanguageList(LanguageItem* dest, i32 destcount) = 0;
	virtual i32 GetIMEConversionModes(ConversionModeItem* dest, i32 destcount) = 0;
	virtual i32 GetIMESentenceModes(SentenceModeItem* dest, i32 destcount) = 0;

	virtual void OnChangeIMEByHandle(i32 handleValue) = 0;
	virtual void OnChangeIMEConversionModeByHandle(i32 handleValue) = 0;
	virtual void OnChangeIMESentenceModeByHandle(i32 handleValue) = 0;

	virtual void OnInputLanguageChanged() = 0;
	virtual void OnIMEStartComposition() = 0;
	virtual void OnIMEComposition(i32 flags) = 0;
	virtual void OnIMEEndComposition() = 0;

	virtual void OnIMEShowCandidates() = 0;
	virtual void OnIMEChangeCandidates() = 0;
	virtual void OnIMECloseCandidates() = 0;
	virtual void OnIMERecomputeModes() = 0;

	virtual i32 GetCandidateListCount() = 0;
	virtual void GetCandidate(i32 num, wchar_t* dest, i32 destSizeBytes) = 0;
	virtual i32 GetCandidateListSelectedItem() = 0;
	virtual i32 GetCandidateListPageSize() = 0;
	virtual i32 GetCandidateListPageStart() = 0;

	//NOTE:  We render our own candidate lists most of the time...
	virtual void SetCandidateWindowPos(i32 x, i32 y) = 0;

	virtual bool GetShouldInvertCompositionString() = 0;
	virtual bool CandidateListStartsAtOne() = 0;

	virtual void SetCandidateListPageStart(i32 start) = 0;

	// Passes in a keycode which allows hitting other mouse buttons w/o cancelling capture mode
	virtual void SetMouseCaptureEx(VPANEL panel, MouseCode captureStartMouseCode) = 0;

	// Because OnKeyCodeTyped uses CallParentFunction and is therefore message based, there's no way
	//  to know if handler actually swallowed the specified keycode.  To get around this, I set a global before calling the
	//  kb focus OnKeyCodeTyped function and if we ever get to a Panel::OnKeyCodeTypes we know that nobody handled the message
	//  and in that case we can post a message to any "unhandled keycode" listeners
	// This will generate an MESSAGE_FUNC_INT( "KeyCodeUnhandled" "code" code ) message to each such listener
	virtual void RegisterKeyCodeUnhandledListener(VPANEL panel) = 0;
	virtual void UnregisterKeyCodeUnhandledListener(VPANEL panel) = 0;

	// Posts unhandled message to all i32erested panels
	virtual void OnKeyCodeUnhandled(i32 keyCode) = 0;

	// Assumes subTree is a child panel of the root panel for the vgui contect
	//  if restrictMessagesToSubTree is true, then mouse and kb messages are only routed to the subTree and it's children and mouse/kb focus
	//   can only be on one of the subTree children, if a mouse click occurs outside of the subtree, and "UnhandledMouseClick" message is sent to unhandledMouseClickListener panel
	//   if it's set
	//  if restrictMessagesToSubTree is false, then mouse and kb messages are routed as normal except that they are not routed down into the subtree
	//   however, if a mouse click occurs outside of the subtree, and "UnhandleMouseClick" message is sent to unhandledMouseClickListener panel
	//   if it's set
	virtual void SetModalSubTree(VPANEL subTree, VPANEL unhandledMouseClickListener, bool restrictMessagesToSubTree = true) = 0;
	virtual void ReleaseModalSubTree() = 0;
	virtual VPANEL GetModalSubTree() = 0;

	// These toggle whether the modal subtree is exclusively receiving messages or conversely whether it's being excluded from receiving messages
	// Sends a "ModalSubTree", state message
	virtual void SetModalSubTreeReceiveMessages(bool state) = 0;
	virtual bool ShouldModalSubTreeReceiveMessages() const = 0;

	virtual VPANEL GetMouseCapture() = 0;

	virtual VPANEL GetMouseFocus() = 0;
};

#endif // SDK_IINPUT_HPP
