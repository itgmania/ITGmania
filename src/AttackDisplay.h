#ifndef AttackDisplay_H
#define AttackDisplay_H

#include "ActorFrame.h"
#include "GameConstantsAndTypes.h"
#include "PlayerState.h"
#include "RageTexturePreloader.h"
#include "Sprite.h"

RString GetAttackPieceName(const RString& attack);

// A graphical display for attacks.
class AttackDisplay : public ActorFrame {
 public:
  AttackDisplay();

  void Init(const PlayerState* player_state);
  void SetAttack(const RString& mod);

  virtual void Update(float delta);

 protected:
  const PlayerState* player_state_;
  Sprite sprite_;
  RageTexturePreloader texture_preload_;
};

#endif  // AttackDisplay_H

/**
 * @file
 * @author Chris Danford (c) 2003
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
