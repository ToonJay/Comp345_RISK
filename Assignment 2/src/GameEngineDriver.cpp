#include "GameEngine.h"

void testStartupPhase() {
	GameEngine ge{};
	ge.startupPhase();
}

void testMainGameLoop() {
	GameEngine ge{};
	ge.startupPhase();
	ge.mainGameLoop();
}

void testOrderExecution() {
	GameEngine ge{};
	ge.startupPhase();
	ge.mainGameLoop();
}