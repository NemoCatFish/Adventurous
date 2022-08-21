outputName = Game
linkers = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

Game: src.cpp
	g++ src.cpp -w -o $(outputName) $(linkers)
	./$(outputName)
	rm -f $(outputName)

windows: src.cpp
	i686-w64-mingw32-g++ src.cpp -Iinclude -Llib -w -Wl,-subsystem,windows -lmingw32 -lSDL2main $(linkers) -o bin/Adventurer

web:
	em++ src.cpp -O2 -s USE_SDL=2 -s LLD_REPORT_UNDEFINED -o web/index.js -s USE_SDL_MIXER=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_OGG=1 -s USE_SDL_TTF=2 --preload-file res