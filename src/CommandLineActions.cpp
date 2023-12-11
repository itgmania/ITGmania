#include "global.h"

#include "CommandLineActions.h"

#include <vector>

#include "DateTime.h"
#include "IniFile.h"
#include "JsonUtil.h"
#include "LuaManager.h"
#include "Preference.h"
#include "ProductInfo.h"
#include "RageFile.h"
#include "RageFileManager.h"
#include "RageUtil.h"
#include "ScreenInstallOverlay.h"
#include "SpecialFiles.h"
#include "XmlFile.h"
#include "XmlFileUtil.h"
#include "arch/Dialog/Dialog.h"
#include "arch/LoadingWindow/LoadingWindow.h"
#include "ver.h"

// only used for Version()
#if defined(_WINDOWS)
#include <conio.h>
#include <windows.h>
#endif

std::vector<CommandLineActions::CommandLineArgs> CommandLineActions::ToProcess;

static void LuaInformation() {
  XNode* pNode = LuaHelpers::GetLuaInformation();
  pNode->AppendAttr("xmlns", "http://www.stepmania.com");
  pNode->AppendAttr("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
  pNode->AppendAttr("xsi:schemaLocation", "http://www.stepmania.com Lua.xsd");

  pNode->AppendChild("Version", std::string(PRODUCT_FAMILY) + product_version);
  pNode->AppendChild("Date", DateTime::GetNowDate().GetString());

  XmlFileUtil::SaveToFile(pNode, "Lua.xml", "Lua.xsl");

  delete pNode;
}

// Print out version information.
//
// NOTE(aj): This function is primarily needed for Windows users.
// macOS and Linux print out version information on the command line
// regardless of any preferences (tested by shakesoda on Mac).
static void Version() {
#if defined(WIN32)
  RString sProductID =
      ssprintf("%s", (std::string(PRODUCT_FAMILY) + product_version).c_str());
  RString sVersion = ssprintf(
      "build %s\nCompile Date: %s @ %s", ::sm_version_git_hash, version_date,
      version_time);

  AllocConsole();
  freopen("CONOUT$", "wb", stdout);
  freopen("CONOUT$", "wb", stderr);

  fprintf(
      stdout, "Version Information:\n%s %s\n", sProductID.c_str(),
      sVersion.c_str());
  fprintf(stdout, "Press any key to exit.");
  _getch();
#endif  // WIN32
}

void CommandLineActions::Handle(LoadingWindow* pLW) {
  CommandLineArgs args;
  for (int i = 0; i < g_argc; ++i) {
    args.argv.push_back(g_argv[i]);
  }
  ToProcess.push_back(args);

  bool bExitAfter = false;
  if (GetCommandlineArgument("ExportLuaInformation")) {
    LuaInformation();
    bExitAfter = true;
  }
  if (GetCommandlineArgument("version")) {
    Version();
    bExitAfter = true;
  }
  if (bExitAfter) {
    exit(0);
  }
}

/*
 * (c) 2006 Chris Danford, Steve Checkoway
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
