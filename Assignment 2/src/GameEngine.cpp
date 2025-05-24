#include "GameEngine.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GameEngine class function definitions 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor
GameEngine::GameEngine()
	: gameState{new GameState{GameState::Start}}, cmdProcessor{new CommandProcessor}, map{new Map}, deck{new Deck}, playersList{new std::vector<Player>} {
	// std::cout << "Called GameEngine default constructor" << std::endl;
}

// Copy constructor
GameEngine::GameEngine(const GameEngine& source)
	: gameState{new GameState{*source.gameState}}, cmdProcessor{new CommandProcessor{*source.cmdProcessor}},
	map{new Map{*source.map}}, deck{new Deck{*source.deck}}, playersList{new std::vector<Player>{*source.playersList}} {
	// std::cout << "Called GameEngine destructor" << std::endl;
}

// Destructor
GameEngine::~GameEngine() {
	delete gameState;
	delete cmdProcessor;
}

// Assignment operator overload
GameEngine& GameEngine::operator=(const GameEngine& rhs) {
	if (this != &rhs) {
		delete gameState;
		delete cmdProcessor;
		gameState = new GameState{*rhs.gameState};
		cmdProcessor = new CommandProcessor{*rhs.cmdProcessor};
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

// Getters
GameState& GameEngine::getGameState() const {
	return *gameState;
}

CommandProcessor& GameEngine::getCommandProcessor() const {
	return *cmdProcessor;
}

// Transitions from one gameState to the next
void GameEngine::transition(const Command& command) {
	switch (*gameState) {
	case GameState::Start:
		*gameState = GameState::Map_Loaded;
		break;
	case GameState::Map_Loaded:
		if (command.getCommand() == "validatemap") {
			*gameState = GameState::Map_Validated;
		}
		break;
	case GameState::Map_Validated:
		*gameState = GameState::Players_Added;
		break;
	case GameState::Players_Added:
		if (command.getCommand() == "gamestart") {
			*gameState = GameState::Assign_Reinforcement;
		}
		break;
	case GameState::Assign_Reinforcement:
		*gameState = GameState::Issue_Orders;
		break;
	case GameState::Issue_Orders:
		if (command.getCommand() == "endissueorders") {
			*gameState = GameState::Execute_Orders;
		}
		break;
	case GameState::Execute_Orders:
		if (command.getCommand() == "endexecorders") {
			*gameState = GameState::Assign_Reinforcement;
		} else if (command.getCommand() == "win") {
			*gameState = GameState::Win;
		}
		break;
	case GameState::Win:
		if (command.getCommand() == "replay") {
			*gameState = GameState::Start;
		} else if (command.getCommand() == "quit") {
			*gameState = GameState::Exit_Program;
		}
		break;
	}
}

void GameEngine::startupPhase() {
	GameState& gs{getGameState()};
	std::list<Command>& commandList{cmdProcessor->getCommandList()};
	std::string cmdString{};
	bool isValid{false};

	while (!isValid) {
		std::cout << "Please choose to get commands from console or file <filename>: ";
		Command& cmd = cmdProcessor->getCommand();
		std::istringstream iss{cmd.getCommand()};
		iss >> cmdString;
		if (cmdString == "file") {
			std::string fileName;
			iss >> fileName;
			delete cmdProcessor;
			cmdProcessor = new FileCommandProcessorAdapter{fileName};
			if (dynamic_cast<FileCommandProcessorAdapter*>(cmdProcessor)->getFileLineReader().getFile()) {
				isValid = true;
			} else {
				delete cmdProcessor;
				cmdProcessor = new CommandProcessor;
				std::cout << "Invalid file name, please try again." << std::endl;
			}
		} else if (cmdString == "console") {
			isValid = true;
		} else {
			std::cout << "Invalid choice, please try again." << std::endl;
		}
	}
	
	while (gs != GameState::Assign_Reinforcement) {
		std::cout << "Current Game State: " << *this << std::endl;
		std::cout << "Please enter a command: ";
		Command& cmd = cmdProcessor->getCommand();
		std::istringstream iss{cmd.getCommand()};
		if (!cmdProcessor->validate(cmd, gs)) {
			std::cout << std::endl << cmd << std::endl;
		} else {
			iss >> cmdString;
			if (cmdString == "loadmap") {
				std::getline(iss, cmdString);
				cmdString.erase(0, 1);
				std::ifstream file{cmdString};
				if (file.is_open()) {
					MapLoader mapLoader{cmdString};
					*map = mapLoader.getMap();
					cmd.saveEffect("\"" + cmdString + "\" loaded successfully.");
					transition(cmd);
				} else {
					cmd.saveEffect("Invalid file name.");
				}
				file.close();
			} else if (cmdString == "validatemap") {
				if (map->validate()) {
					cmd.saveEffect("Map validated successfully.");
					transition(cmd);
				} else {
					cmd.saveEffect("Invalid Map.");
				}
			} else if (cmdString == "addplayer") {
				std::getline(iss, cmdString);
				cmdString.erase(0, 1);
				if (playersList->size() < 6) {
					playersList->emplace_back(Player{cmdString});
					cmd.saveEffect("Player \"" + cmdString + "\" added.");
					transition(cmd);
				} else {
					cmd.saveEffect("Player limit reached.");
				}
			} else if (cmdString == "gamestart") {
				if (playersList->size() > 1) {

					size_t numOfPlayerTerritories{map->getMainMap().size() / 2 - (map->getMainMap().size() / 2) % playersList->size()};
					std::cout << numOfPlayerTerritories << std::endl;
					std::vector<int> territoryDistribution;
					for (size_t i = 0; i < numOfPlayerTerritories; i++) {
						territoryDistribution.emplace_back(i % playersList->size());
					}

					std::srand(unsigned(std::time(0)));
					std::random_shuffle(playersList->begin(), playersList->end());
					std::random_shuffle(territoryDistribution.begin(), territoryDistribution.end());

					int count{0};
					for (const auto& territory : map->getMainMap()) {
						if (count < numOfPlayerTerritories) {
							playersList->at(territoryDistribution.at(count)).getPlayerTerritories().push_back(&territory.first);
							count++;
						} else {
							break;
						}
					}

					for (const Player& p : *playersList) {
						p.getReinforcementPool() = 50;
						deck->draw(p.getPlayerHand());
						deck->draw(p.getPlayerHand());
						std::cout << p << std::endl;
					}
					transition(cmd);
				} else {
					cmd.saveEffect("Cannot start game. Not enough players.");
				}
			}
			std::cout << std::endl << cmd << std::endl;
		}
	}
}
