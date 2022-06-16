/* Rgb2Hires
* Copyright (C) 2021 Christophe Meneboeuf <christophe@xtof.info>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <thread>
#include <atomic>
#include <mutex>

#include "ImageQuantized.h"
#include "HiRes.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace Display
{    

  struct rgba8Bits_t
  {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0xff;
  };

  using Block = std::array<rgba8Bits_t, 14>;
  using Line = std::array<Block, 40>;
  using Screen = std::array<Line, 192 * 2>;

  class Window
  {
  public:
    Window() = default;
    ~Window();
      
    static Window* GetInstance();

    void display(const std::string& path, const uint8_t* hiresblob);

  private:
    bool init();
    void sdlError(const std::string& msg);

    std::thread* _pThread = nullptr; //< to survey filechange
    std::mutex   _mutex;
    std::atomic_bool _isFileModified = false;
    std::atomic_bool _stopFileSurvey = false;

    static Window* S_pInstance;
    static const int SCALE = 2;

    SDL_Window*    _pWindow = nullptr;
    SDL_Renderer*  _pRenderer  = nullptr;
    SDL_Texture*   _pTexture = nullptr;
  };

}

#endif

