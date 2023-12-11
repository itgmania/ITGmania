#ifndef DisplaySpec_H
#define DisplaySpec_H

#include <set>

#include "RageTypes.h"

struct DisplayMode {
  // Width (in pixels) of the display in this mode
  unsigned int width;
  // Height (in pixels) of the display in this mode
  unsigned int height;
  // Refresh rate, in hz, of the display for the given mode
  // MacOS Quartz Display Services provides rate as double,
  // winapi DEVMODE uses 32bit unsigned int (still gives rate as hz),
  // RandR gives 32 bit pixel clock, which is then divided by width*height.
  double refresh_rate;
  // Bits-per-pixel is *not* a property of the DisplayMode for our purposes:
  // bit-depth is going to be a property of the OpenGL/D3D context, not a
  // display configuration

  bool operator<(const DisplayMode& other) const {
/** @brief A quick way to compare the two DisplayResolutions. */
#define COMPARE(x) \
  if (x != other.x) return x < other.x;
    COMPARE(width);
    COMPARE(height);
    COMPARE(refresh_rate);
#undef COMPARE
    return false;
  }

  // Lua
  void PushSelf(lua_State* L);
};

// The dimensions of the program.
class DisplaySpec {
 public:
  // Construct a specification for the display with the given ID, which supports
  // the given modes, and is currently using the specified mode with the
  // specified logical screen bounds
  DisplaySpec(
      const std::string& id, const std::string& name,
      const std::set<DisplayMode>& modes, const DisplayMode& curMode,
      const RectI& curBounds, const bool isVirtual = false)
      : id_(id),
        name_(name),
        modes_(modes),
        cur_mode_is_active_(true),
        cur_mode_(curMode),
        rect_bounds_(curBounds),
        is_virtual_(isVirtual) {
    if (modes_.find(curMode) == modes_.end()) {
      // This is an error, make a failing assertion with a descriptive error
      // message
      std::stringstream msgStream;
      msgStream << "DisplaySpec current mode (" << curMode.width << "x"
                << curMode.height << "@" << curMode.refresh_rate
                << ") not in given list of supported modes: ";
      for (auto& m : modes) {
        msgStream << m.width << "x" << m.height << "@" << m.refresh_rate << ", ";
      }
      auto msg = msgStream.str();
      // Drop the trailing ", "
      msg.resize(msg.size() - 2);

      ASSERT_M(false, msg.c_str());
    }
  }

  // Construct a specification for the display with the given ID, which supports
  // the given modes, and is currently disabled (has no active mode)
  DisplaySpec(
      const std::string id, const std::string name,
      const std::set<DisplayMode> modes, const bool isVirtual = false)
      : id_(id),
        name_(name),
        modes_(modes),
        cur_mode_is_active_(false),
        cur_mode_({}),
        is_virtual_(isVirtual) {}

  // Create a specification for a display supporting a single (and currently
  // active) mode
  DisplaySpec(std::string id, std::string name, DisplayMode mode)
      : id_(id),
        name_(name),
        cur_mode_is_active_(true),
        cur_mode_(mode),
        is_virtual_(false) {
    modes_.insert(mode);
    rect_bounds_ = RectI(0, 0, mode.width, mode.height);
  }

  DisplaySpec(const DisplaySpec& other) = default;
  DisplaySpec& operator=(const DisplaySpec& other) = default;

  std::string name() const { return name_; }

  std::string id() const { return id_; }

  const std::set<DisplayMode>& supportedModes() const { return modes_; }

  // Return a pointer to the currently active display mode, or nullptr if
  // display is inactive
  //
  // Note that inactive *does not* necessarily mean unusable. E.g., in X11,
  // an output can be enabled/disabled by an application by
  // connecting/disconnecting a crtc
  const DisplayMode* currentMode() const {
    return cur_mode_is_active_ ? &cur_mode_ : nullptr;
  }

  const RectI& currentBounds() const { return rect_bounds_; }

  bool isVirtual() const { return is_virtual_; }

  // Determine if one DisplaySpec compares less than the other.
  //
  // Used to enforce a consistent ordering of displays, e.g. for consistent
  // option presentation. Also allows DisplaySpec to be placed in a std::set
  bool operator<(const DisplaySpec& other) const { return id_ < other.id(); }

  // Lua
  void PushSelf(lua_State* L);

 private:
  // Unique identifier of the display
  std::string id_;
  // "Human-readable" display name
  std::string name_;
  // Modes supported by this display
  std::set<DisplayMode> modes_;
  // currently configured mode, if available
  bool cur_mode_is_active_;
  DisplayMode cur_mode_;
  // The current bounds of this display in global display coordinate space
  RectI rect_bounds_;
  // Flag is "true" when display represents a logical display like an X screen
  // or the Win32 "Virtual screen"
  bool is_virtual_;
};
//f The collection of DisplaySpec available within the program.
typedef std::set<DisplaySpec> DisplaySpecs;
// Lua
DisplaySpecs* pushDisplaySpecs(lua_State* L, const DisplaySpecs& specs);

#endif  // DisplaySpec_H

/**
 * @file
 * @author Chris Danford (c) 2001-2005
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
