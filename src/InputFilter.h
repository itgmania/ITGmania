// InputFilter - Checks RageInput and generates a list of InputEvents,
// representing button presses, releases, and repeats.
#ifndef INPUT_FILTER_H
#define INPUT_FILTER_H

#include <vector>

#include "RageInputDevice.h"

enum InputEventType {
  // The device was just pressed.
  IET_FIRST_PRESS,

  // The device is auto-repeating. This event is guaranteed to be sent only
  // between IET_FIRST_PRESS and IET_RELEASE pairs.
  IET_REPEAT,

  // The device is no longer pressed. Exactly one IET_RELEASE event will be
  // sent for each IET_FIRST_PRESS.
  IET_RELEASE,

  NUM_InputEventType,
  InputEventType_Invalid
};

const RString& InputEventTypeToString(InputEventType input_event_type);
const RString& InputEventTypeToLocalizedString(InputEventType input_event_type);
LuaDeclareType(InputEventType);

struct InputEvent {
  InputEvent() : type_(IET_FIRST_PRESS) {}

  DeviceInput device_input_;
  InputEventType type_;

  // A list of all buttons that were pressed at the time of this event:
  DeviceInputList button_state_;
};

struct MouseCoordinates {
  float x;
  float y;
  float z;
};

class RageMutex;
struct ButtonState;

class InputFilter {
 public:
  void ButtonPressed(const DeviceInput& device_input);
  void SetButtonComment(
			const DeviceInput& device_input, const RString& comment = "");
  void ResetDevice(InputDevice device);

  InputFilter();
  ~InputFilter();
  void Reset();
  void Update(float delta);

  void SetRepeatRate(float repeat_rate);
  void SetRepeatDelay(float delay);
  void ResetRepeatRate();
  void ResetKeyRepeat(const DeviceInput& device_input);
  void RepeatStopKey(const DeviceInput& device_input);

  // If aButtonState is nullptr, use the last reported state.
  bool IsBeingPressed(
      const DeviceInput& device_input,
      const DeviceInputList* button_state = nullptr) const;
  float GetSecsHeld(
      const DeviceInput& device_input,
      const DeviceInputList* button_state = nullptr) const;
  float GetLevel(
      const DeviceInput& device_input,
      const DeviceInputList* button_state = nullptr) const;
  RString GetButtonComment(const DeviceInput& device_input) const;

  void GetInputEvents(std::vector<InputEvent>& event_out);
  void GetPressedButtons(std::vector<DeviceInput>& array) const;

  // cursor
  void UpdateCursorLocation(float x, float y);
  void UpdateMouseWheel(float z);
  float GetCursorX() { return mouse_coords_.x; }
  float GetCursorY() { return mouse_coords_.y; }
  float GetMouseWheel() { return mouse_coords_.z; }

  // Lua
  void PushSelf(lua_State* L);

 private:
  void CheckButtonChange(
			ButtonState& button_state, DeviceInput device_input,
			const RageTimer& now);
  void ReportButtonChange(
			const DeviceInput& device_input, InputEventType input_event_type);
  void MakeButtonStateList(std::vector<DeviceInput>& input_out) const;

  std::vector<InputEvent> queue_;
  RageMutex* queue_mutex_;
  MouseCoordinates mouse_coords_;

  InputFilter(const InputFilter& rhs);
  InputFilter& operator=(const InputFilter& rhs);
};

// Global and accessible from anywhere in our program.
extern InputFilter* INPUTFILTER;

#endif  // INPUT_FILTER_H

/*
 * (c) 2001-2004 Chris Danford
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons to
 * whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF
 * THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR HOLDERS
 * INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL INDIRECT
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
 * OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 */
