#ifndef ANNOUNCER_MANAGER_H
#define ANNOUNCER_MANAGER_H

#include "global.h"

#include <vector>

#include "RageTypes.h"

// The commentators who say seemlingly random things during gameplay.
class AnnouncerManager {
 public:
  AnnouncerManager();
  ~AnnouncerManager();

  // Retrieve the announcer names.
  void GetAnnouncerNames(std::vector<RString>& AddTo);
  // Determine if the specified announcer exists.
  // True if it exists, false otherwise.
  bool DoesAnnouncerExist(RString sAnnouncerName);
  // Switch to a new specified announcer.
  void SwitchAnnouncer(RString sNewAnnouncerName);
  // Retrieve the current announcer's name.
  RString GetCurAnnouncerName() const { return m_sCurAnnouncerName; };
  void NextAnnouncer();

  RString GetPathTo(RString sFolderName);
  bool HasSoundsFor(RString sFolderName);

  // Lua
  void PushSelf(lua_State* L);

 protected:
  static RString GetAnnouncerDirFromName(RString sAnnouncerName);
  RString GetPathTo(RString AnnouncerPath, RString sFolderName);
  // the current announcer's name.
  RString m_sCurAnnouncerName;
};

// Global and accessible from anywhere in our program
extern AnnouncerManager* ANNOUNCER;  

#endif  // ANNOUNCER_MANAGER_H

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
