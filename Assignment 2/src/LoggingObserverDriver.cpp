#include "LoggingObserver.h"
#include "GameEngine.h"

void testLoggingObserver() {
	GameEngine ge{};
	ge.startupPhase();
	ge.mainGameLoop();
}