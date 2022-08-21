std::vector<characterStatic*> staticCharacters;
std::vector<character*> characters;
std::vector<button*> buttons;
lvlMap* lvlScene;
lvlMap* lvlSceneEx;
	
Mix_Chunk* switchSound;

slider* SoundSlide;

key* k;
bool drawKey = false;

int currentLvl = 0;

void loadLvl(){
	if (!switchSound){
		switchSound = Mix_LoadWAV("res/soundEffects/switch.ogg");
		Mix_VolumeChunk(switchSound, 50);		

		SoundSlide = new slider({330, 10});
	}
	switch (currentLvl){
		case 0:
			buttons = {new button("res/kenney/ui/panels.png", {170, 120, 120, 30}, "Play")};
			lvlScene = new lvlMap("res/kenney/world/lvl0.png", {{0, 32* 6, 32 * 15, 32 * 3}});
			staticCharacters = {
				new characterStatic("res/kenney/malePerson.png", vec2(20, 32*3), lvlScene->collisionRect),
				new characterStatic("res/kenney/maleAdventurer.png", vec2(100, 32*3), lvlScene->collisionRect),
				new characterStatic("res/kenney/femalePerson.png", vec2(300, 32*3), lvlScene->collisionRect),
				new characterStatic("res/kenney/femaleAdventurer.png", vec2(380, 32*3), lvlScene->collisionRect)
			};
			break;
		case 1:
			lvlScene = new lvlMap("res/kenney/world/lvl1.png", {{0, 32* 6, 32 * 15, 32 * 3}});
			characters = {
				new character("res/kenney/malePerson.png", vec2(20, 32*3), lvlScene->collisionRect)};
			break;

		case 2:
			lvlScene = new lvlMap("res/kenney/world/lvl2.png", {{0, 32* 6, 32 * 7, 32 * 3}, 
																{32*9, 32* 6, 32 * 6, 32 * 3}});
			characters = {
				new character("res/kenney/malePerson.png", vec2(20, 32*3), lvlScene->collisionRect)};
			break;

		case 3:
			lvlScene = new lvlMap("res/kenney/world/lvl3.png", {{0, 32* 6, 32 * 6, 32 * 3}, 
																{32*10, 32* 6, 32 * 6, 32 * 3}}
			);

			lvlSceneEx = new lvlMap("res/kenney/world/lvl3(ex).png", {{32 * 6, 32 * 6, 32 * 4, 32 * 1}});

			characters = {
				new character("res/kenney/malePerson.png", vec2(20, 32*3), lvlScene->collisionRect),
				new character("res/kenney/maleAdventurer.png", vec2(100, 32*3), lvlScene->collisionRect, 200, false)};
			
			k = new key({32 * 11, 32 * 5});
			drawKey = true;

			break;

		case 4:
			lvlScene = new lvlMap("res/kenney/world/lvl4.png", {{0, 32* 6, 32 * 6, 32 * 3}, 
																{32*11, 32* 7, 32 * 5, 32 * 2}}
			);

			lvlSceneEx = new lvlMap("res/kenney/world/lvl4(ex).png", {{32 * 6, 32 * 6, 32 * 2, 32 * 1}});

			characters = {
				new character("res/kenney/malePerson.png", vec2(20, 32*3), lvlScene->collisionRect),
				new character("res/kenney/maleAdventurer.png", vec2(100, 32*3), lvlScene->collisionRect, 200, false)};
			
			k = new key({32 * 11, 32 * 5});
			drawKey = true;

			break;

		case 5:
			lvlScene = new lvlMap("res/kenney/world/lvl5.png", {{0, 32* 7, 32 * 5, 32 * 1},
																{32*6, 32* 1, 32 * 4, 32 * 1}, 
																{32*11, 32* 7, 32 * 4, 32 * 1}}
			);

			lvlSceneEx = new lvlMap("res/kenney/world/lvl5(ex).png", {{32 * 6, 32 * 8, 32 * 4, 32 * 1}});

			characters = {
				new character("res/kenney/malePerson.png", vec2(20, 32*3), lvlScene->collisionRect),
				new character("res/kenney/femalePerson.png", vec2(100, 32*3), lvlScene->collisionRect, 120, false, 13)};
			
			k = new key({32 * 11, 32 * 5});
			drawKey = true;

			break;
	}
}

void lvlClean(){

	characters.clear();

	buttons.clear();
}

void lvlLoop(){
	SoundSlide->loop();
	bool keyPick = false;
	lvlScene->loop();
	if (drawKey){
		k->draw();
	}
	bool switchCharacter = false;
	if (currentLvl){
		if (keyMap[SDLK_r] == 1 && sizeof(characters) > 1){
			keyMap[SDLK_r] = 2;
			switchCharacter = true;
			Mix_PlayChannel(-1, switchSound, 0);
			std::cout << Mix_GetError() << std::endl;
		}
		if (keyMap[SDLK_e]){
			keyPick = true;
		}
		int index = 0;
		for (character* &p: characters){
			if (p->imgRect.y > HEIGHT){
				delete p;
				characters.clear();
				loadLvl();
				return;
			}
			if (switchCharacter){
				if (p->current){
					p->current = false;
				}
				else{
					p->current = true;
				}
			}
			if (keyPick && drawKey){
				if (abs(p->imgRect.x - k->imgRect.x) < 30){
					keyPick = false;
					drawKey = false;
					
					for (character* &p: characters){
						for (SDL_Rect r: lvlSceneEx->collisionRect){
							p->mapCollision.push_back(r);
						}
					}
				}
			}
			p->loop();

			if (currentLvl < 5 && p->imgRect.x > WIDTH && index == 0){
				delete p;
				characters.clear();
				currentLvl += 1;
				loadLvl();
			}
			else if (currentLvl == 5 && p->imgRect.x > WIDTH && index == 0){
				delete p;
				characters.clear();
				currentLvl = 0;
				loadLvl();	
			}
		index++;
		}
	}
	else{
		for (characterStatic* &p: staticCharacters){
			p->loop();
		}
	}

	if (!drawKey && lvlSceneEx){
		lvlSceneEx->loop();
	}

	for (button* &b: buttons){
		b->loop();
		if (b->action){
			currentLvl += 1;
			loadLvl();
			delete b;
			*b;
			buttons.clear();
		}
	}
}