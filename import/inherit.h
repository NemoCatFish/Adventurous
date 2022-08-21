class textLabel{
private:
	SDL_Texture* tex;
	SDL_Rect rect;

public:
	textLabel(std::string text_, int size, SDL_Point pos){
		TTF_Font* font = TTF_OpenFont("res/kenney/ui/fonts/kenvector_future.ttf", size);
		SDL_Surface* surface = TTF_RenderText_Solid(font, text_.c_str(), {0, 0, 0, 0});
		if (!surface){
			std::cout << TTF_GetError() << std::endl;
			return;
		}
		tex = SDL_CreateTextureFromSurface(r, surface);
		SDL_FreeSurface(surface);
		SDL_QueryTexture(tex, nullptr, nullptr, &rect.w, &rect.h);
		TTF_CloseFont(font);
		rect.x = pos.x;
		rect.y = pos.y;
	}
	~textLabel(){
		SDL_DestroyTexture(tex);
		*tex;
	}

	void draw(){
		SDL_RenderCopy(r, tex, nullptr, &rect);
	}
};

class sprite{
private:
	SDL_Texture* tex;
protected:
	SDL_RendererFlip flip = SDL_FLIP_NONE;
public:
	SDL_Rect clipRect = {0, 0, 0, 0};
	SDL_Rect imgRect = {0, 0, 0, 0};
	sprite(std::string src){
		SDL_Surface* surface = IMG_Load(src.c_str());
		if (!surface){
			std::cout << IMG_GetError() << std::endl;
			return;
		}

		tex = SDL_CreateTextureFromSurface(r, surface);
		
		if (!tex){
			std::cout << SDL_GetError() << std::endl;
			return;
		}
		SDL_FreeSurface(surface);

		SDL_QueryTexture(tex, nullptr, nullptr, &imgRect.w, &imgRect.h);
	}

	~sprite(){
		SDL_DestroyTexture(tex);
	}

	void draw(){
		SDL_RenderCopyEx(r, tex, &clipRect, &imgRect, 0, nullptr, flip);
	}

};

class key: public sprite{
public:
	key(SDL_Point pos): sprite("res/kenney/ui/key.png"){
		clipRect.w = imgRect.w;
		clipRect.h = imgRect.h;
		imgRect.w /= 2;
		imgRect.h /= 2;
		imgRect.x = pos.x;
		imgRect.y = pos.y;
	}
};

class lvlMap: public sprite{
public:
	std::vector<SDL_Rect> collisionRect;
	bool drawEx = true;
	lvlMap(std::string src, std::vector<SDL_Rect> collisionRect_)
	:sprite(src){
		clipRect.w = imgRect.w;
		clipRect.h = imgRect.h;
		imgRect.w = 32*15;
		imgRect.h = 32*9;
		collisionRect = collisionRect_;

	}
	void loop(){
		draw();
	}
};

class button: public sprite{
private:
	Mix_Chunk* clickSound = Mix_LoadWAV("res/kenney/ui/sounds/click1.ogg");
	Mix_Chunk* releaseSound = Mix_LoadWAV("res/kenney/ui/sounds/click2.ogg");
	textLabel* text;
	double time = 0;
	bool pressed = false;
	void checkInput(){
		if (SDL_PointInRect(&releasePos, &imgRect)){
			action = true;
		}
		if (!mousePressed && pressed){
			pressed = false;
			Mix_PlayChannel(-1, releaseSound, 1);
			clipRect.y = 0;
		}
		else if (SDL_PointInRect(&clickPos, &imgRect) && !pressed){
			pressed = true;
			clipRect.y = clipRect.h;
			Mix_PlayChannel(-1, clickSound, 1);
		}
	}

	void animate(){
		time += delta;
		if (time > 0.5){
			time = 0;
			if ( (clipRect.x + clipRect.w) > clipRect.w){
				clipRect.x = 0;
			}
			else{
				clipRect.x += clipRect.w;
			}
		}
	}

public:
	bool action = false;

	button(std::string src, SDL_Rect rect_, std::string text_): sprite(src){
		imgRect = rect_;
		clipRect = {0, 0, 190, 49};
		text = new textLabel(text_, 25, {imgRect.x + 22, imgRect.y});
	}

	~button(){
		delete text;
		*text;
		Mix_FreeChunk(clickSound);
		Mix_FreeChunk(releaseSound);
	}

	void loop(){
		checkInput();
		animate();
		draw();
		text->draw();
	}
};

class slider{
private:
	sprite* container;
	sprite* bar;
public:
	slider(vec2 pos){
		container = new sprite("res/kenney/ui/panels.png");
		container->clipRect = {0, 49*3, 190, 45};
		int w = container->clipRect.w/1.5;
		int h = container->clipRect.h/1.5;
		container->imgRect = {(int)pos.x, (int)pos.y, w, h};
	
		bar = new sprite("res/kenney/ui/panels.png");
		bar->clipRect = {0, 0, 190, 49};
		bar->imgRect = {pos.x, pos.y, container->imgRect.w/2, (bar->clipRect.h - 10)/1.5};
		Mix_VolumeMusic(50);
		}

	void loop(){
		container->draw();
		bar->draw();
		if (mousePressed && SDL_PointInRect(&clickPos, &container->imgRect)){
			bar->imgRect.w = clickPos.x - container->imgRect.x;
			Mix_VolumeMusic(((float)bar->imgRect.w / container->imgRect.w) * 100.0f);
			Mix_Volume(4, ((float)bar->imgRect.w / container->imgRect.w) * 100.0f);
		}
	}
};


