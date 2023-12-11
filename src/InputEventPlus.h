#ifndef INPUT_EVENT_PLUS_H
#define INPUT_EVENT_PLUS_H

#include "GameInput.h"
#include "InputFilter.h"
#include "PlayerNumber.h"

// Holds a device input plus Game/Menu translations.
class InputEventPlus {
 public:
  InputEventPlus()
      : device_input_(),
        game_input_(),
        type_(IET_FIRST_PRESS),
        menu_input_(GameButton_Invalid),
        pn_(PLAYER_INVALID),
        multiplayer_(MultiPlayer_Invalid),
        input_list_() {}
  DeviceInput device_input_;
  GameInput game_input_;
  InputEventType type_;
  GameButton menu_input_;
  PlayerNumber pn_;
  MultiPlayer multiplayer_;
  DeviceInputList input_list_;
};

struct AlternateMapping {
  GameInput input_main;
  GameInput input_alt;
};

#endif  // INPUT_EVENT_PLUS_H

/**
 * @file
 * @author Chris Danford (c) 2001-2004
 * @section LICENSE
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
