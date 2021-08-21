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

Game::~Game() {
}

bool Game::IsRunning() const {
	return isRunning;
}

void Game::Init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Error initializing SDL" << std::endl;
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

	init();

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

		input(sdlEvent);

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

	update(deltaTime);

	return;
}

void Game::Draw() {
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderClear(renderer);

	draw();

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
