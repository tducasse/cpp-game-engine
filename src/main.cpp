#define SOL_ALL_SAFETIES_ON 1


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <iostream>
#include "game/game.h"
#include <sol/sol.hpp>

bool quit = false;
void MainLoop(void*);

int main(int argc, char* argv[])
{
	sol::state lua;
	lua.open_libraries(sol::lib::base);
	sol::protected_function_result result = lua.script_file("lua/main.lua");


	sol::function init = lua["init"];
	sol::function input = lua["input"];
	sol::function update = lua["update"];
	sol::function draw = lua["draw"];

	Game game = Game(init, input, update, draw);

	game.Init();

#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop_arg(MainLoop, &game, 0, 1);
#else
	while (!quit)
	{
		MainLoop(&game);
	}
#endif

	game.Destroy();

	return 0;
}


void MainLoop(void* arg) {
	Game game = *static_cast<Game*>(arg);
	game.ProcessInput();
	game.Update();
	game.Draw();
	if (!game.IsRunning()) {
		quit = true;
	}
#ifdef __EMSCRIPTEN__
	if (quit) {
		emscripten_cancel_main_loop();
	}
#endif
}


