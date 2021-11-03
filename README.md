# Game Engine

Just a sample game engine - mostly as a learning experiment.
Built in C++ on top of SDL, scriptable in Lua through Sol2 bindings. Cross platform thanks to CMake, and compiles to wasm and js via emscripten.


## Work on it
- `git clone REPO`
- `cd game_engine`
- `git submodule update --init`
- open the folder with Visual Studio, it should detect CMake automatically
- right-click on CMakeLists.txt, then click "Debug" to run the project
- the `sample` folder, which contains a test game, is symlinked alongside the compiled executable, so modify `main.lua` and it should be fine

## Compiling a web version
### Prerequisites
Make sure these dependencies are installed and in your terminal PATH (easy enough with [chocolatey](https://chocolatey.org/)):
- CMake (`choco install -y cmake --installargs 'ADD_CMAKE_TO_PATH=User'`)
- Ninja (`choco install -y ninja`)
- Python (`choco install -y python --version=3.9.5`)

### Install and activate emsdk components
- open a developer command prompt (in Visual Studio, Tools->Command Line->Developer Command Prompt)
- run the following commands:
```sh
cd externals\emsdk
emsdk install latest
emsdk activate latest
emcc --check
```
At this point, there might be some errors. Run `emsdk list` to see if you can install them through emsdk.
For example:
```sh
emsdk install fastcomp-clang-e1.38.30-64bit
emsdk activate fastcomp-clang-e1.38.30-64bit
```

### Actually compile
In the same terminal:
```sh
# back to the root folder
emcmake cmake . -B web-build -G Ninja
# this is the command that you will re-run every time you want to recompile
cmake --build web-build
```
Since WASM is only possible when files are served from a server, run `python -m http.server` (in another terminal) to quickly serve your files, and navigate to [localhost:8000](http://localhost:8000) to test it.


### Recompiling later
```sh
# reactivate the environment
externals\emsdk\emsdk_env.bat
# recompile the sources
cmake --build web-build
```

## Externals
- Lua, as a submodule, will be built from source and compiled to a static lib
- SDL, as a submodule, will be build from source and compiled to a static lib
- Sol, as a single header
- emsdk, as a submodule
