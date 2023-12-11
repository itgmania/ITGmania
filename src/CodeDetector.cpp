#include "global.h"

#include "CodeDetector.h"

#include "Game.h"
#include "GameState.h"
#include "InputEventPlus.h"
#include "InputQueue.h"
#include "OptionRowHandler.h"
#include "PlayerOptions.h"
#include "PlayerState.h"
#include "RageLog.h"
#include "RageUtil.h"
#include "ThemeManager.h"

const char* CodeNames[] = {
    "PrevSteps1",
    "PrevSteps2",
    "NextSteps1",
    "NextSteps2",
    "NextSort1",
    "NextSort2",
    "NextSort3",
    "NextSort4",
    "ModeMenu1",
    "ModeMenu2",
    "Mirror",
    "Backwards",
    "Left",
    "Right",
    "Shuffle",
    "SuperShuffle",
    "NextTransform",
    "NextScrollSpeed",
    "PreviousScrollSpeed",
    "NextAccel",
    "NextEffect",
    "NextAppearance",
    "NextTurn",
    "Reverse",
    "HoldNotes",
    "Mines",
    "Dark",
    "Hidden",
    "RandomVanish",
    "CancelAll",
    "NextTheme",
    "NextTheme2",
    "NextAnnouncer",
    "NextAnnouncer2",
    "NextGroup",
    "PrevGroup",
    "SaveScreenshot1",
    "SaveScreenshot2",
    "CancelAllPlayerOptions",
    "BackInEventMode",
    "CloseCurrentFolder",
    "PrevOptionsList",
    "NextOptionsList"};
XToString(Code);

static InputQueueCode g_CodeItems[NUM_Code];

bool CodeDetector::EnteredCode(GameController controller, Code code) {
  return g_CodeItems[code].EnteredCode(controller);
}

void CodeDetector::RefreshCacheItems(RString code_class) {
  if (code_class == "") {
    code_class = "CodeDetector";
  }
  FOREACH_ENUM(Code, c) {
    InputQueueCode& item = g_CodeItems[c];
    const RString code_name = CodeToString(c);
    const RString button_names = THEME->GetMetric(code_class, code_name);

    item.Load(button_names);
  }
}

bool CodeDetector::EnteredCloseFolder(GameController controller) {
  return EnteredCode(controller, CODE_CLOSE_CURRENT_FOLDER);
}

bool CodeDetector::EnteredNextGroup(GameController controller) {
  return EnteredCode(controller, CODE_NEXT_GROUP);
}

bool CodeDetector::EnteredPrevGroup(GameController controller) {
  return EnteredCode(controller, CODE_PREV_GROUP);
}

bool CodeDetector::EnteredPrevSteps(GameController controller) {
  return EnteredCode(controller, Code_PrevSteps1) ||
         EnteredCode(controller, Code_PrevSteps2);
}

bool CodeDetector::EnteredNextSteps(GameController controller) {
  return EnteredCode(controller, Code_NextSteps1) ||
         EnteredCode(controller, Code_NextSteps2);
}

bool CodeDetector::EnteredNextSort(GameController controller) {
  return EnteredCode(controller, CODE_NEXT_SORT1) ||
         EnteredCode(controller, CODE_NEXT_SORT2) ||
         EnteredCode(controller, CODE_NEXT_SORT3) ||
         EnteredCode(controller, CODE_NEXT_SORT4);
}

bool CodeDetector::EnteredModeMenu(GameController controller) {
  return EnteredCode(controller, CODE_MODE_MENU1) ||
         EnteredCode(controller, CODE_MODE_MENU2);
}

bool CodeDetector::EnteredPrevOpList(GameController controller) {
  return EnteredCode(controller, CODE_PREV_OPLIST);
}
bool CodeDetector::EnteredNextOpList(GameController controller) {
  return EnteredCode(controller, CODE_NEXT_OPLIST);
}

#define TOGGLE(v, a, b) \
  if (v != a) {         \
    v = a;              \
	} else {              \
    v = b;              \
	}
#define FLOAT_TOGGLE(v) \
  if (v != 1.f) {       \
    v = 1.f;            \
  } else {              \
    v = 0.f;            \
	}

#define INCREMENT_SCROLL_SPEED(s) \
  (s == 0.5f)    ? s = 0.75f      \
  : (s == 0.75f) ? s = 1.0f       \
  : (s == 1.0f)  ? s = 1.5f       \
  : (s == 1.5f)  ? s = 2.0f       \
  : (s == 2.0f)  ? s = 3.0f       \
  : (s == 3.0f)  ? s = 4.0f       \
  : (s == 4.0f)  ? s = 5.0f       \
  : (s == 5.0f)  ? s = 8.0f       \
                 : s = 0.5f;
#define DECREMENT_SCROLL_SPEED(s) \
  (s == 0.75f)  ? s = 0.5f        \
  : (s == 1.0f) ? s = 0.75f       \
  : (s == 1.5f) ? s = 1.0f        \
  : (s == 2.0f) ? s = 1.5f        \
  : (s == 3.0f) ? s = 2.0f        \
  : (s == 4.0f) ? s = 3.0f        \
  : (s == 5.0f) ? s = 4.0f        \
  : (s == 8.0f) ? s = 4.0f        \
                : s = 8.0f;

bool CodeDetector::DetectAndAdjustMusicOptions(GameController controller) {
  PlayerNumber pn = INPUTMAPPER->ControllerToPlayerNumber(controller);

  for (int c = CODE_MIRROR; c <= CODE_CANCEL_ALL; ++c) {
    Code code = (Code)c;

    PlayerOptions po =
        GAMESTATE->player_state_[pn]->m_PlayerOptions.GetPreferred();

    if (EnteredCode(controller, code)) {
      switch (code) {
        case CODE_MIRROR:
          po.ToggleOneTurn(PlayerOptions::TURN_MIRROR);
          break;
        case CODE_BACKWARDS:
          po.ToggleOneTurn(PlayerOptions::TURN_BACKWARDS);
          break;
        case CODE_LEFT:
          po.ToggleOneTurn(PlayerOptions::TURN_LEFT);
          break;
        case CODE_RIGHT:
          po.ToggleOneTurn(PlayerOptions::TURN_RIGHT);
          break;
        case CODE_SHUFFLE:
          po.ToggleOneTurn(PlayerOptions::TURN_SHUFFLE);
          break;
        case CODE_SUPER_SHUFFLE:
          po.ToggleOneTurn(PlayerOptions::TURN_SUPER_SHUFFLE);
          break;
        case CODE_NEXT_TRANSFORM:
          po.NextTransform();
          break;
        case CODE_NEXT_SCROLL_SPEED:
          INCREMENT_SCROLL_SPEED(po.m_fScrollSpeed);
          break;
        case CODE_PREVIOUS_SCROLL_SPEED:
          DECREMENT_SCROLL_SPEED(po.m_fScrollSpeed);
          break;
        case CODE_NEXT_ACCEL:
          po.NextAccel();
          break;
        case CODE_NEXT_EFFECT:
          po.NextEffect();
          break;
        case CODE_NEXT_APPEARANCE:
          po.NextAppearance();
          break;
        case CODE_NEXT_TURN:
          po.NextTurn();
          break;
        case CODE_REVERSE:
          po.NextScroll();
          break;
        case CODE_HOLDS:
          TOGGLE(
              po.m_bTransforms[PlayerOptions::TRANSFORM_NOHOLDS], true, false);
          break;
        case CODE_MINES:
          TOGGLE(
              po.m_bTransforms[PlayerOptions::TRANSFORM_NOMINES], true, false);
          break;
        case CODE_DARK:
          FLOAT_TOGGLE(po.m_fDark);
          break;
        case CODE_CANCEL_ALL:
          GAMESTATE->GetDefaultPlayerOptions(po);
          break;
        case CODE_HIDDEN:
          ZERO(po.m_fAppearances);
          po.m_fAppearances[PlayerOptions::APPEARANCE_HIDDEN] = 1;
          break;
        case CODE_RANDOMVANISH:
          ZERO(po.m_fAppearances);
          po.m_fAppearances[PlayerOptions::APPEARANCE_RANDOMVANISH] = 1;
          break;
        default:
          break;
      }

      GAMESTATE->player_state_[pn]->m_PlayerOptions.Assign(
          ModsLevel_Preferred, po);

      return true;  // don't check any more
    }
  }

  return false;
}

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
