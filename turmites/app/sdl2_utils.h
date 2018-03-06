#pragma once

#include <functional>
#include <string_view>

#include "SDL.h"

namespace turmites_sim::sdl2_utils {

std::function<void(SDL_Window*)> getWindowDeleter();
std::function<void(SDL_Renderer*)> getRendererDeleter();

void logError(std::string_view causedBy);

}
