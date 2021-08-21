#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <sol/sol.hpp>


inline constexpr unsigned int FPS = 60;
inline constexpr unsigned int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
private:
	bool isRunning;
	int ticksPreviousFrame;
	SDL_Window* window;
	SDL_Renderer* renderer;
	static int r;
	static int g;
	static int b;
	static int a;
	void ChangeColor();
	sol::function init;
	sol::function input;
	sol::function update;
	sol::function draw;


public:
	Game(sol::function init, sol::function input, sol::function update, sol::function draw);
	~Game();
	void Init();
	void ProcessInput();
	void Update();
	void Draw();
	void Destroy();
	bool IsRunning() const;

	static int windowWidth;
	static int windowHeight;
};

#endif