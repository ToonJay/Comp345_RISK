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

// Where the map is loaded and validated; where the players are added; and where territory distribution is done.
// This is where the game setup happens and it can be done either via console or file.
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
					size_t numOfPlayerTerritories{map->getGameMap().size() / 2 - map->getGameMap().size() / 2 % playersList->size()};
					std::cout << numOfPlayerTerritories << std::endl;
					std::vector<int> territoryDistribution;
					for (size_t i = 0; i < numOfPlayerTerritories; i++) {
						territoryDistribution.emplace_back(i % playersList->size());
					}

					std::srand(unsigned(std::time(0)));
					std::random_shuffle(playersList->begin(), playersList->end());
					std::srand(unsigned(std::time(0)));
					std::random_shuffle(territoryDistribution.begin(), territoryDistribution.end());

					int count{0};
					for (const auto& territory : map->getGameMap()) {
						if (count < numOfPlayerTerritories) {
							playersList->at(territoryDistribution.at(count)).getPlayerTerritories().emplace(&territory.first);
							territory.first.getOwner() = playersList->at(territoryDistribution.at(count)).getPlayerName();
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
	cmdProcessor = new CommandProcessor;
	mainGameLoop();
}

// The main game loop.
// The game loops through these three phases until it's done.
void GameEngine::mainGameLoop() {
	reinforcementPhase();
	issueOrdersPhase();
	executeOrdersPhase();
}

// This is where players get their reinforcements.
// The reinforcements they'll get is the number of territories divided by 3, with a minimum of 3.
// They also get extra reinforcements if they own a complete continent depending on the continent's bonus value.
void GameEngine::reinforcementPhase() {
	std::cout << "Reinforcement Phase\n" << std::endl;

	int reinforcements;
	for (const Player& p : *playersList) {
		reinforcements = p.getPlayerTerritories().size() / 3;
		if (reinforcements <= 3) {
			p.getReinforcementPool() += 3;
		} else {
			p.getReinforcementPool() += reinforcements;
		}
	}

	for (const auto& continent : map->getContinents()) {
		std::string owner{map->getGameMap().find(*continent.second.begin())->first.getOwner()};
		if (owner != "Neutral") {
			bool allSameOwner{true};
			for (const auto& territory : continent.second) {
				if (owner != map->getGameMap().find(territory)->first.getOwner()) {
					allSameOwner = false;
					break;
				}
			}

			if (allSameOwner) {
				std::cout << "Continent of " << continent.first << " is fully owned by " << owner << std::endl;
				std::cout << owner << " receives additional reinforcements of " << map->getContinentBonuses().at(continent.first) << std::endl;
				for (const Player& player : *playersList) {
					if (owner == player.getPlayerName()) {
						player.getReinforcementPool() += map->getContinentBonuses().at(continent.first);
					}
				}
			}
		}
	}

	for (const Player& p : *playersList) {
		std::cout << p << std::endl;
	}
}

// This is where players issue all their orders in round-robin fashion until each player has no more orders to give
void GameEngine::issueOrdersPhase() {
	std::cout << "Issue Orders Phase\n" << std::endl;
	std::vector<bool> finishedOrders(playersList->size(), false);
	int endCount{0};

	while (endCount < playersList->size()) {
		for (size_t i = 0; i < playersList->size(); i++) {
			if (finishedOrders.at(i) == false) {
				playersList->at(i).issueOrder(*cmdProcessor, *map);
				if (cmdProcessor->getCommandList().back().getCommand() == "endissueorders") {
					finishedOrders.at(i) = true;
					endCount++;
				}
			}
		}
	}
	std::cout << std::endl;
}

// This is where the players' orders get validated and executed in round-robin fashion
// However, it will first go through all deploy orders from all players first
void GameEngine::executeOrdersPhase() {
	std::cout << "Execute Orders Phase\n" << std::endl;
	std::vector<bool> finishedOrders(playersList->size(), false);
	int endCount{0};
	while (endCount < playersList->size()) {
		for (size_t i = 0; i < playersList->size(); i++) {
			if (finishedOrders.at(i) == false) {
				Player& p{playersList->at(i)};
				std::cout << p.getPlayerName() << " Order:" << std::endl;
				std::cout << **p.getPlayerOrdersList().getOrders().begin() << std::endl;
				p.getPlayerOrdersList().remove(0);
				if (p.getPlayerOrdersList().getOrders().size() == 0) {
					finishedOrders.at(i) = true;
					endCount++;
				}
			}
		}
	}
	exit;
}