#define SOL_ALL_SAFETIES_ON 1

extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}


#include <sol/sol.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <iostream>
#include "game/game.h"

bool quit = false;
void MainLoop(void*);


int main(int argc, char* argv[])
{
	sol::state lua;
	lua.open_libraries(sol::lib::base);

	sol::table twng = lua.create_table("twng");

	Game game;

	twng.set_function("draw_image", &Game::DrawImage, game);

	lua.script_file("scripts/main.lua");

	sol::function init = lua["twng"]["init"];
	sol::function input = lua["twng"]["input"];
	sol::function update = lua["twng"]["update"];
	sol::function draw = lua["twng"]["draw"];


	game.SetCallbacks(init, input, update, draw);


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


