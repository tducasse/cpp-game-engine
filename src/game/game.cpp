#include <iostream>
#include "game.h"

int Game::windowWidth = 640;
int Game::windowHeight = 480;
int Game::r = 100;
int Game::g = 100;
int Game::b = 100;
int Game::a = 255;

Game::Game(sol::function init, sol::function input, sol::function update, sol::function draw) {
	isRunning = false;
	window = NULL;
	renderer = NULL;
	ticksPreviousFrame = 0;
	this->init = init;
	this->update = update;
	this->draw = draw;
	this->input = input;
}

void Game::SetCallbacks(sol::function init, sol::function input, sol::function update, sol::function draw) {
	this->init = init;
	this->update = update;
	this->draw = draw;
	this->input = input;
}

Game::Game() {
	isRunning = false;
	window = NULL;
	renderer = NULL;
	ticksPreviousFrame = 0;
}

Game::~Game() {
}

bool Game::IsRunning() const {
	return isRunning;
}

void Game::Init() {
	if (SDL_Init(SDL_INIT_EVERYTHING & ~(SDL_INIT_SENSOR | SDL_INIT_HAPTIC)) != 0) {
		std::cerr << "Error initializing SDL : " << SDL_GetError() << std::endl;
		return;
	}

	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_SHOWN
	);
	if (!window) {
		std::cerr << "Error creating SDL window." << std::endl;
		return;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		std::cerr << "Error creating SDL renderer." << std::endl;
		return;
	}

	isRunning = true;

	if (init) {
		auto result = init();
		if (!result.valid()) {
			sol::error err = result;
			std::cout << "An error occurred: " << err.what() << std::endl;
		}
	}


	return;
}


void Game::ProcessInput() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
		case SDL_QUIT:
			isRunning = false;
			break;
		case SDL_KEYUP:
			Game::ChangeColor();
			SDL_Log("key pressed");
			break;

		}

		if (input) {
			auto result = input(sdlEvent);
			if (!result.valid()) {
				sol::error err = result;
				std::cout << "An error occurred: " << err.what() << std::endl;
			}
		}

	}
}

void Game::Update() {
	int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - ticksPreviousFrame);
	if (timeToWait > 0 && timeToWait <= MILLISECS_PER_FRAME) {
		SDL_Delay(timeToWait);
	}
	double deltaTime = (SDL_GetTicks() - ticksPreviousFrame) / 1000.0f;
	deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;
	ticksPreviousFrame = SDL_GetTicks();

	if (update) {
		auto result = update(deltaTime);
		if (!result.valid()) {
			sol::error err = result;
			std::cout << "An error occurred: " << err.what() << std::endl;
		}
	}


	return;
}


void Game::DrawImage(Game* game, std::string path) {
	SDL_Surface* surface = SDL_LoadBMP(path.c_str());
	if (surface == NULL) {
		std::cout << "An error occurred: " << SDL_GetError() << std::endl;
	}


	SDL_Texture* texture = SDL_CreateTextureFromSurface(game->renderer, surface);
	if (texture == NULL) {
		std::cout << "An error occurred: " << SDL_GetError() << std::endl;
	}

	SDL_RenderCopy(game->renderer, texture, NULL, NULL);
	SDL_FreeSurface(surface);
}

void Game::Draw() {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderClear(renderer);

	if (draw) {
		auto result = draw(this);
		if (!result.valid()) {
			sol::error err = result;
			std::cout << "An error occurred: " << err.what() << std::endl;
		}
	}

	SDL_RenderPresent(renderer);

	return;
}

void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int GetRandomColorNumber() {
	int min = 0;
	int max = 255;
	return min + (std::rand() % (max - min + 1));
}

void Game::ChangeColor() {
	r = GetRandomColorNumber();
	g = GetRandomColorNumber();
	b = GetRandomColorNumber();
}
