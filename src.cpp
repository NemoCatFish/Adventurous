#include <SDL2/SDL.h> //#include SDL.h>
#include <SDL2/SDL_image.h> //#include <SDL_image.h>
#include <SDL2/SDL_ttf.h> //#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h> //#include <SDL2/SDL_mixer.h>


#include <time.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>
//#include <emscripten.h>

std::map<int, int> keyMap;

const int WIDTH = 460;
const int HEIGHT = WIDTH * 9/16;
double delta = 0;
SDL_Renderer* r;
std::vector<char> v;

#include "import/math.h"

bool mousePressed = false;
SDL_Point clickPos = {0, 0};
SDL_Point releasePos = {0, 0};

#include "import/inherit.h"
#include "import/characters.h"
#include "import/LvlManager.h"


class Game{
private:
	double elapsedTime = 0;
	SDL_Window* window;
	SDL_Event e;
	Mix_Music* theme;

	void init(){

		SDL_Init(SDL_INIT_VIDEO);
		IMG_Init( IMG_INIT_PNG );
		TTF_Init();
		Mix_Init( MIX_INIT_OGG | MIX_INIT_MP3 );
	
		window = SDL_CreateWindow("Adventurous", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
		r = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		SDL_SetRenderDrawColor(r, 5, 154, 244, 255);
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		loadLvl();
		theme = Mix_LoadMUS("res/theme.mp3");
		Mix_PlayMusic(theme, -1);


	}

	void inputCheck(){
		while (SDL_PollEvent(&e)){
			switch (e.type){
				case SDL_QUIT:
					running = false;
//					emscripten_cancel_main_loop();
					break;
				case SDL_KEYDOWN:
					if (!keyMap[e.key.keysym.sym]){
						keyMap[e.key.keysym.sym] = 1;
					}
					break;
				case SDL_KEYUP:
					keyMap[e.key.keysym.sym] = 0;
					break;
				case SDL_MOUSEBUTTONDOWN:
					clickPos.x = e.button.x;
					clickPos.y = e.button.y;
					releasePos.x = 0;
					releasePos.y = 0;
					mousePressed = true;
					break;
				case SDL_MOUSEBUTTONUP:
					clickPos.x = 0;
					clickPos.y = 0;
					releasePos.x = e.button.x;
					releasePos.y = e.button.y;
					mousePressed = false;
					break;
			}
		}
	}

	void updateDelta(){
		delta = (SDL_GetTicks() - elapsedTime) / 1000.0f;
		elapsedTime = SDL_GetTicks();
	}

public:
	bool running = true;
	Game(){
		init();
//		loop();
//		clean();
	}
	void loop(){
//		while (running){
			updateDelta();
			inputCheck();
			SDL_RenderClear(r);

			lvlLoop();

			SDL_RenderPresent(r);
//		}
	}

	void clean(){
		lvlClean();
		SDL_DestroyRenderer(r);
		r = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
		Mix_Quit();
	}
};

Game* g;

void loop(){
	while (g->running){
		g->loop();
	}
}

int main(int argc, char *argv[]) {
	srand(time(nullptr));

	g = new Game();
	loop();
//	emscripten_set_main_loop(loop, 0, false);
	g->clean();
	return 0;
}