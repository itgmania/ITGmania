#include "global.h"

#include "ActiveAttackList.h"

#include "PlayerOptions.h"
#include "PlayerState.h"

#include <vector>

ActiveAttackList::ActiveAttackList() {}

void ActiveAttackList::Init(const PlayerState* player_state) {
  player_state_ = player_state;
}

void ActiveAttackList::Update(float delta) {
  bool time_to_refresh =
      IsFirstUpdate() || // check this before running Actor::Update()
      player_state_->m_bAttackBeganThisUpdate ||
      player_state_->m_bAttackEndedThisUpdate;

  BitmapText::Update(delta);

  if (time_to_refresh) {
    Refresh();
  }
}

void ActiveAttackList::Refresh() {
  const AttackArray& attacks = player_state_->m_ActiveAttacks;

  std::vector<RString> vsThemedMods;
  for (unsigned i = 0; i < attacks.size(); ++i) {
    const Attack& attack = attacks[i];

    if (!attack.bOn) {
      // Attack hasn't started yet.
      continue;
    }

    if (!attack.bShowInAttackList) {
      continue;
    }

    PlayerOptions po;
    po.FromString(attack.sModifiers);
    po.GetLocalizedMods(vsThemedMods);
  }

  RString s = join("\n", vsThemedMods);

  // BitmapText will not rebuild vertices if these strings are the same.
  this->SetText(s);
}

/*
 * (c) 2004 Chris Danford
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
