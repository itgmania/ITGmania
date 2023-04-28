#ifndef ControllerStateDisplay_H
#define ControllerStateDisplay_H

#include "ActorFrame.h"
#include "ActorUtil.h"
#include "AutoActor.h"
#include "GameInput.h"
#include "PlayerNumber.h"
#include "RageInput.h"

enum ControllerStateButton {
  ControllerStateButton_UpLeft,
  ControllerStateButton_UpRight,
  ControllerStateButton_Center,
  ControllerStateButton_DownLeft,
  ControllerStateButton_DownRight,
  NUM_ControllerStateButton
};

// Show the button state of a controller.
class ControllerStateDisplay : public ActorFrame {
 public:
  ControllerStateDisplay();
  void LoadMultiPlayer(RString type, MultiPlayer multiplayer);
  void LoadGameController(RString type, GameController game_controller);
  virtual void Update(float delta);
  bool IsLoaded() const { return is_loaded_; }

  virtual ControllerStateDisplay* Copy() const;

  // Lua
  virtual void PushSelf(lua_State* L);

 protected:
  void LoadInternal(RString sType, MultiPlayer mp, GameController gc);
  MultiPlayer multiplayer_;

  bool is_loaded_;
  AutoActor smart_pointer_frame_;
  struct Button {
    Button() {
      device_input_.MakeInvalid();
      game_input_.MakeInvalid();
    }

    AutoActor smart_pointer_;
    DeviceInput device_input_;
    GameInput game_input_;
  };
  Button buttons_[NUM_ControllerStateButton];

  InputDeviceState last_input_device_state_;
};

#endif  // ControllerStateDisplay_H

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
