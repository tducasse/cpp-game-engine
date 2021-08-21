#define SOL_ALL_SAFETIES_ON 1

#include <iostream>
#include "game/game.h"
#include <sol/sol.hpp>



int main(int argc, char* argv[])
{
	sol::state lua;
	lua.open_libraries(sol::lib::base);
	sol::protected_function_result result = lua.script_file("main.lua");

	sol::function update = lua["update"];
	sol::function draw = lua["draw"];
	sol::function init = lua["init"];
	sol::function input = lua["input"];

	Game game;

	game.Init(init);

	while (game.IsRunning()) {
		game.ProcessInput(input);
		game.Update(update);
		game.Draw(draw);
	}

	game.Destroy();

	return 0;
}