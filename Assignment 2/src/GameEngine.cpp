#include "GameEngine.h"

// Default constructor
GameEngine::GameEngine() 
	: gameState{new GameState{GameState::Start}}, cmdProcessor{new CommandProcessor} {
}

// Copy constructor
GameEngine::GameEngine(const GameEngine& source)
	: gameState{new GameState{*source.gameState}}, cmdProcessor{new CommandProcessor{*source.cmdProcessor}} {
}

// Destructor
GameEngine::~GameEngine() {
	delete gameState;
	delete cmdProcessor;
}

// Assignment operator overload
GameEngine& GameEngine::operator=(const GameEngine& rhs) {
	if (this != &rhs) {
		*gameState = *rhs.gameState;
	}
	return *this;
}

// Stream insertion operator overload
std::ostream& operator<<(std::ostream& os, const GameEngine& obj) {
	switch (obj.getGameState()) {
		case GameState::Start: return os << "Start";
		case GameState::Map_Loaded: return os << "Map Loaded";
		case GameState::Map_Validated: return os << "Map Validated";
		case GameState::Players_Added: return os << "Players Added";
		case GameState::Assign_Reinforcement: return os << "Assign Reinforcement";
		case GameState::Issue_Orders: return os << "Issue Orders";
		case GameState::Execute_Orders: return os << "Execute Orders";
		case GameState::Win: return os << "Win";
		default: return os << "Error";
	}
}

// Getter
GameState& GameEngine::getGameState() const {
	return *gameState;
}

// Starts and controls the game loop
// Asks for command input, if it's valid depending on the state, it goes through.
void GameEngine::gameUI() {
	while (*gameState != GameState::Exit_Program) {
		Command& c = cmdProcessor->getCommand();
		cmdProcessor->validate(c, getGameState());
	}
}
