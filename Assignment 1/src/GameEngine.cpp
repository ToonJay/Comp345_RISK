#include "GameEngine.h"

// Default constructor
GameEngine::GameEngine() 
	: gameState{new GameState{GameState::Start}} {
}

// Copy constructor
GameEngine::GameEngine(const GameEngine& source)
	: gameState{new GameState{*source.gameState}} {
}

// Destructor
GameEngine::~GameEngine() {
	delete gameState;
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
	switch (*obj.gameState) {
		case GameState::Start: return os << "Start";
		case GameState::Map_Loaded: return os << "Map Loaded";
		case GameState::Map_Validated: return os << "Map Validated";
		case GameState::Players_Added: return os << "Players Added";
		case GameState::Assign_Reinforcement: return os << "Assign Reinforcement";
		case GameState::Issue_Orders: return os << "Issue Orders";
		case GameState::Execute_Orders: return os << "Execute Orders";
		case GameState::Win: return os << "Win";
		default: return os << "None";
	}
}

// Getter
GameState& GameEngine::GetGameState() const {
	return *gameState;
}

// Starts and controls the game loop
// Asks for command input, if it's valid depending on the state, it goes through.
void GameEngine::GameLoop() {
	while (command != "end" || *gameState != GameState::Win) {
		std::cout << "\nCurrent State: " << *this << std::endl;
		std::cout << "Please enter a command: ";
		std::cin >> command;

		switch (*gameState) {
		case GameState::Start:
			if (command == "loadmap") {
				std::cout << "Loading map..." << std::endl;
				*gameState = GameState::Map_Loaded;
				std::cout << "Map loaded." << std::endl;
			} else {
				std::cout << "Invalid command for current state."  << std::endl;
			}
			break;
		case GameState::Map_Loaded:
			if (command == "loadmap") {
				std::cout << "Loading map..." << std::endl;
				std::cout << "Map loaded." << std::endl;
			} else if (command == "validatemap") {
				std::cout << "Validating map..." << std::endl;
				*gameState = GameState::Map_Validated;
				std::cout << "Map validated." << std::endl;
			} else {
				std::cout << "Invalid command for current state." << std::endl;
			}
			break;
		case GameState::Map_Validated:
			if (command == "addplayer") {
				std::cout << "Adding player..." << std::endl;
				*gameState = GameState::Players_Added;
				std::cout << "Player added." << std::endl;
			} else {
				std::cout << "Invalid command for current state." << std::endl;
			}
			break;
		case GameState::Players_Added:
			if (command == "addplayer") {
				std::cout << "Adding player..." << std::endl;
				std::cout << "Player added." << std::endl;
			} else if (command == "assigncountries") {
				std::cout << "Assigning countries..." << std::endl;
				*gameState = GameState::Assign_Reinforcement;
				std::cout << "Countries assigned." << std::endl;
			} else{
				std::cout << "Invalid command for current state." << std::endl;
			}
			break;
		case GameState::Assign_Reinforcement:
			if (command == "issueorder") {
				std::cout << "Issuing order..." << std::endl;
				*gameState = GameState::Issue_Orders;
				std::cout << "Order issued." << std::endl;
			} else {
				std::cout << "Invalid command for current state." << std::endl;
			}
			break;
		case GameState::Issue_Orders:
			if (command == "issueorder") {
				std::cout << "Issuing order..." << std::endl;
				std::cout << "Order issued." << std::endl;
			} else if (command == "endissueorders") {
				*gameState = GameState::Execute_Orders;
				std::cout << "Finished issueing orders." << std::endl;
			} else {
				std::cout << "Invalid command for current state." << std::endl;
			}
			break;
		case GameState::Execute_Orders:
			if (command == "execorder") {
				std::cout << "Executing order..." << std::endl;
				std::cout << "Order executed." << std::endl;
			} else if (command == "endexecorders") {
				*gameState = GameState::Assign_Reinforcement;
				std::cout << "Finished executing orders." << std::endl;
			} else if (command == "win") {
				*gameState = GameState::Win;
				std::cout << "CONGRATULATIONS! YOU WON!" << std::endl;
				std::cout << "If you'd like to play another game, type play. Otherwise, type end." << std::endl;
			} else {
				std::cout << "Invalid command for current state." << std::endl;
			}
			break;
		case GameState::Win:
			if (command == "play") {
				*gameState = GameState::Start;
			} else if (command == "end") {
				std::cout << "Goodbye!" << std::endl;
			} else {
				std::cout << "Invalid command for current state." << std::endl;
			}
			break;
		default:
			std::cout << "Invalid command." << std::endl;
		}
	}
}
