#include "sdl2_utils.h"

#include <iostream>

namespace turmites_sim::sdl2_utils {

std::function<void(SDL_Window*)> getWindowDeleter() {
	return [](SDL_Window* window) {
		SDL_DestroyWindow(window); };
}

std::function<void(SDL_Renderer*)> getRendererDeleter() {
	return [](SDL_Renderer* renderer) {
		SDL_DestroyRenderer(renderer); };
}

void logError(std::string_view causedBy) {
	std::cout << causedBy << " Error: " << SDL_GetError() << std::endl;
}

}
