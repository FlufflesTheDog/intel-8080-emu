#include "emulator.hpp"
#include "ioport.hpp"
#include <SDL2/SDL.h>
#include <chrono>

struct ScreenData {
	static constexpr int width = 224;
	static constexpr int height = 256;
	SDL_Window* window;
	SDL_Surface* surface;
	ScreenData()
	{
		auto initCode = SDL_Init(SDL_INIT_EVERYTHING);
		if (initCode < 0) {
			throw std::runtime_error("Unable to init SDL");
		}
		window = SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
		surface = SDL_GetWindowSurface(window);
	};

	void drawScreen(unsigned char* ram)
	{
		SDL_LockSurface(surface);
		auto pixels = static_cast<unsigned*>(surface->pixels);
		auto bit = [](int n, int bit) -> OpUtils::byte {
			n >>= bit;
			return (n & 1) * 0xFF;
		};
		for (int i = 0; i < width * height; ++i) {
			auto color = bit(ram[i / 8], i % 8);
			auto x = i / height;
			auto y = height - 1 - i % height;
			pixels[y * width + x] = SDL_MapRGB(surface->format, color, color, color);
		}
		pixels[0] = SDL_MapRGB(surface->format, 0x32, 0x32, 0x32);
		pixels[3] = SDL_MapRGB(surface->format, 0x32, 0x32, 0x32);
		SDL_UnlockSurface(surface);
		SDL_UpdateWindowSurface(window);
	}

	~ScreenData()
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
};
int main() {
	State emulator("invaders/invaders");
	emulator.devices = std::make_unique<Devices::SpaceInvadersIO>();
	bool running = emulator.IsValid();
	SDL_Event InputEvent;
	auto nextDraw = std::chrono::steady_clock::now();
	bool halfDone = false;
	ScreenData renderer;
	while (running) {
		while (SDL_PollEvent(&InputEvent)) {
			if (InputEvent.type == SDL_QUIT)
				goto done;
		}
		auto now = std::chrono::steady_clock::now();
		if (now > nextDraw) {
			nextDraw = now + std::chrono::milliseconds(16);
			emulator.Interrupt(2);
			if (halfDone) //Normally screen would update twice for frame, alternating between top and bottom
				//but this is simplier and probably faster on modern systems
				renderer.drawScreen(emulator.Memory.get() + 0x2400);
			halfDone = !halfDone;
		}
		//SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));

		running = emulator.StepOpCode();
	}
done:;
}
