
class character: public sprite{
private:
	double time = 0;
	int inputDirection = 0;
	int direction = 1;

	int speed = 120;
	int gravity = 30;
	int id = 1;
	int jumpForce = 10;

	std::vector<Mix_Chunk*> walkSounds = {Mix_LoadWAV("res/soundEffects/walk1.ogg"),
										Mix_LoadWAV("res/soundEffects/walk2.ogg"),
										Mix_LoadWAV("res/soundEffects/walk3.ogg")};

	SDL_Rect testCollision;

	void animate(){
		if (direction < 0){
			flip = SDL_FLIP_HORIZONTAL;
		}
		else{
			flip = SDL_FLIP_NONE;
		}
		if (velocity.y < -1){
			clipRect.y = 0;
			clipRect.x = clipRect.w;
		}
		else if (velocity.y > 1){
			clipRect.y = 0;
			clipRect.x = clipRect.w * 2;
		}
		else if (inputDirection){
			int index = rand() % 3;
			int i = 0;
			if (!Mix_Playing(4)){
				for (Mix_Chunk* s: walkSounds){
					if (i == index){
						Mix_PlayChannel(4, s, 1);
						break;
					}
					i++;
				}
			}
			time += delta;
			clipRect.y = clipRect.h * 4;
			if (time > 0.1){
				time = 0;
				if (clipRect.x < clipRect.w *7){
					clipRect.x += clipRect.w;
				}
				else{
					clipRect.x = 0;
				}
			}
		}
		else{
			clipRect.y = 0;
			clipRect.x = 0;
		}
	}

	void move(){
		if (current){
			inputDirection = (keyMap[SDLK_RIGHT] | keyMap[SDLK_d]) - (keyMap[SDLK_LEFT] | keyMap[SDLK_a]);
		}
		else{
			inputDirection = 0;
		}

		testCollision = {imgRect.x + 9, (int)(imgRect.y + 40 + velocity.y + gravity * delta), imgRect.w - 20, imgRect.h - 40};

		velocity.y += gravity * delta;

		for (SDL_Rect r: mapCollision){
			if (SDL_HasIntersection(&testCollision, &r)){
				velocity.y = 0;
				break;
			}
		}

		if (current && velocity.y == 0 && keyMap[SDLK_SPACE] | keyMap[SDLK_UP] | keyMap[SDLK_w]){
			velocity.y = -jumpForce;
		}


		testCollision = {(int)(imgRect.x + 9 + inputDirection * speed * delta, imgRect.y + 20), imgRect.w - 20, imgRect.h - 20};
		direction = inputDirection;
		velocity.x = inputDirection * speed * delta;
		for (SDL_Rect r: mapCollision){
			if (SDL_HasIntersection(&testCollision, &r)){
				velocity.x = 0;
				break;
			}
		}

		if (velocity.x < 0 && imgRect.x <= 0){
			velocity.x = 0;
		}

		imgRect.x += velocity.x;
		imgRect.y += velocity.y;
	}
public:
	std::vector<SDL_Rect> mapCollision;
	vec2 velocity = vec2(0, 0);
	bool current = true;
	character(std::string src, vec2 pos, std::vector<SDL_Rect> mapCollision_,
		int speed_ = 120, bool current_ = true, int jumpForce_ = 9): sprite(src){
		clipRect.w = imgRect.w/9;
		clipRect.h = imgRect.h/5;
		imgRect.w = 64;
		imgRect.h = 64 * clipRect.h/clipRect.w;
		imgRect.x = pos.x;
		imgRect.y = pos.y;
		mapCollision = mapCollision_;

		jumpForce = jumpForce_;
		speed = speed_;
		current = current_;

		Mix_Volume(4, 50);
	}

	void loop(){
		move();
		animate();
		draw();
	}
};

class characterStatic: public sprite{
private:
	double time = 0;
	double waitTime = 0;
	vec2 velocity = vec2(0, 0);
	int gravity = 30;

	std::vector<SDL_Rect> mapCollision;
	SDL_Rect testCollision;

	void animate(){
		if (waitTime == 0){
			waitTime = rand() % 5 + 2;
		}
		if (velocity.y < -2){
			clipRect.y = 0;
			clipRect.x = clipRect.w;
		}
		else if (velocity.y > 2){
			clipRect.y = 0;
			clipRect.x = clipRect.w * 2;
		}

		else{
			time += delta;
			clipRect.y = 0;
			clipRect.x = 0;
		}
	}

	void move(){
		testCollision = {imgRect.x + 9, (int)(imgRect.y + 20 + velocity.y + gravity * delta), imgRect.w - 20, imgRect.h - 20};

		velocity.y += gravity * delta;

		for (SDL_Rect r: mapCollision){
			if (SDL_HasIntersection(&testCollision, &r)){
				velocity.y = 0;
				break;
			}
		}

		if (time >= waitTime){
			waitTime = rand() % 5 + 2;
			time = 0;
			velocity.y = -10;
		}

		imgRect.y += velocity.y;

	}
public:
	characterStatic(std::string src, vec2 pos, std::vector<SDL_Rect> mapCollision_): sprite(src){
		clipRect.w = imgRect.w/9;
		clipRect.h = imgRect.h/5;
		imgRect.w = 64;
		imgRect.h = 64 * clipRect.h/clipRect.w;
		imgRect.x = pos.x;
		imgRect.y = pos.y;
		mapCollision = mapCollision_;
	}

	void loop(){
		move();
		animate();
		draw();
	}
};