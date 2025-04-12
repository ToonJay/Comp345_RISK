#include "Map.h"

void testLoadMaps() {
	std::cout << "***Map: World.map***" << std::endl;
	MapLoader loadedMap{"./src/Maps/World.map"};
	std::cout << loadedMap << std::endl;
	std::cout << "***Map: USA.map***" << std::endl;
	MapLoader loadedMap2{"./src/Maps/USA.map"};
	std::cout << loadedMap2 << std::endl;
	std::cout << "***Map: 001_I72_Ghtroc 720.map***" << std::endl;
	MapLoader loadedMap3{"./src/Maps/001_I72_Ghtroc 720.map"};
	std::cout << loadedMap3 << std::endl;
}