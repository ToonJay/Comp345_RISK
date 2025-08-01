#include "GameEngine.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GameEngine class function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Default constructor
GameEngine::GameEngine()
	: gameState{new GameState{GameState::Start}}, cmdProcessor{nullptr}, mapLoader{nullptr}, deck {new Deck}, playersList{new std::vector<Player>},
	playerLookup{new std::unordered_map<std::string, Player*>}, diplomacyMap{new std::unordered_map<std::string, std::string>}, logObserver{new LogObserver} {
	attach(*logObserver);
	cmdProcessor = new CommandProcessor{logObserver};
	// //std::cout << "Called GameEngine default constructor" << std::endl;
}

// Copy constructor
GameEngine::GameEngine(const GameEngine& source)
	: gameState{new GameState{*source.gameState}}, cmdProcessor{new CommandProcessor{*source.cmdProcessor}},
	mapLoader{new MapLoader{*source.mapLoader}}, deck{new Deck{*source.deck}}, playersList{new std::vector<Player>{*source.playersList}},
	playerLookup{new std::unordered_map<std::string, Player*>{*source.playerLookup}}, 
	diplomacyMap{new std::unordered_map<std::string, std::string>{*source.diplomacyMap}}, logObserver{new LogObserver{*source.logObserver}} {
	attach(*logObserver);
	// //std::cout << "Called GameEngine destructor" << std::endl;
}

// Destructor
GameEngine::~GameEngine() {
	delete gameState;
	delete cmdProcessor;
	delete mapLoader;
	delete deck;
	delete playersList;
	delete playerLookup;
	delete diplomacyMap;
	delete logObserver;
}

// Assignment operator overload
GameEngine& GameEngine::operator=(const GameEngine& rhs) {
	if (this != &rhs) {
		delete gameState;
		delete cmdProcessor;
		delete mapLoader;
		delete deck;
		delete playersList;
		delete playerLookup;
		delete diplomacyMap;
		delete logObserver;
		gameState = new GameState{*rhs.gameState};
		cmdProcessor = new CommandProcessor{*rhs.cmdProcessor};
		mapLoader = new MapLoader{*rhs.mapLoader};
		deck = new Deck{*rhs.deck};
		playersList = new std::vector<Player>{*rhs.playersList};
		playerLookup = new std::unordered_map<std::string, Player*>{*rhs.playerLookup};
		diplomacyMap = new std::unordered_map<std::string, std::string>{*rhs.diplomacyMap};
		logObserver = new LogObserver{*rhs.logObserver};
	}
	return *this;
}

// Stream insertion operator overload
std::ostream& operator<<(std::ostream& os, const GameEngine& obj) {
	return os << obj.gameStateToString();
}

// Getters
GameState& GameEngine::getGameState() {
	return *gameState;
}

CommandProcessor& GameEngine::getCommandProcessor() {
	return *cmdProcessor;
}

MapLoader& GameEngine::getMapLoader() {
	return *mapLoader;
}

Deck& GameEngine::getDeck() {
	return *deck;
}

Player& GameEngine::getPlayerByName(const std::string& name) {
	return *playerLookup->at(name);
}

std::unordered_map<std::string, std::string>& GameEngine::getDiplomacyMap() {
	return *diplomacyMap;
}

LogObserver& GameEngine::getLogObserver() {
	return *logObserver;
}

const GameState& GameEngine::getGameState() const {
	return *gameState;
}

const CommandProcessor& GameEngine::getCommandProcessor() const {
	return *cmdProcessor;
}

const MapLoader& GameEngine::getMapLoader() const {
	return *mapLoader;
}

const Deck& GameEngine::getDeck() const {
	return *deck;
}

const std::unordered_map<std::string, Player*>& GameEngine::getPlayerLookup() const {
	return *playerLookup;
}

const Player& GameEngine::getPlayerByName(const std::string& name) const {
	return *playerLookup->at(name);
}

const std::unordered_map<std::string, std::string>& GameEngine::getDiplomacyMap() const {
	return *diplomacyMap;
}

const LogObserver& GameEngine::getLogObserver() const {
	return *logObserver;
}

// Transitions from one gameState to the next
void GameEngine::transition(const Command& command) {
	switch (*gameState) {
	case GameState::Start:
		if (command.getCommand().substr(0, 7) == "loadmap") {
			*gameState = GameState::Map_Loaded;
		}
		break;
	case GameState::Map_Loaded:
		if (command.getCommand() == "validatemap") {
			*gameState = GameState::Map_Validated;
		}
		break;
	case GameState::Map_Validated:
		if (command.getCommand().substr(0, 9) == "addplayer") {
			*gameState = GameState::Players_Added;
			break;
		}
	case GameState::Players_Added:
		if (command.getCommand() == "gamestart") {
			*gameState = GameState::Assign_Reinforcement;
		}
		break;
	case GameState::Assign_Reinforcement:
		if (command.getCommand() == "issueorder") {
			*gameState = GameState::Issue_Orders;
		}
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
	notify(*this);
}

std::string GameEngine::gameStateToString() const {
	switch (*gameState) {
	case GameState::Start: return "Start";
	case GameState::Map_Loaded: return "Map Loaded";
	case GameState::Map_Validated: return "Map Validated";
	case GameState::Players_Added: return "Players Added";
	case GameState::Assign_Reinforcement: return "Assign Reinforcement";
	case GameState::Issue_Orders: return "Issue Orders";
	case GameState::Execute_Orders: return "Execute Orders";
	case GameState::Win: return "Win";
	default: return "Error";
	}
}

// Where the mapLoader is loaded and validated; where the players are added; and where territory distribution is done.
// This is where the game setup happens and it can be done either via console or file.
void GameEngine::startupPhase() {
	GameState& gs{getGameState()};
	const std::list<Command>& commandList{cmdProcessor->getCommandList()};
	std::string cmdString{};
	bool isValid{false};

	while (!isValid) {
		std::cout << "Please choose to get commands from console or file <filename>: ";
		const Command& cmd = cmdProcessor->getCommand();
		std::istringstream iss{cmd.getCommand()};
		iss >> cmdString;
		if (cmdString == "file") {
			std::string fileName;
			iss >> fileName;
			delete cmdProcessor;
			cmdProcessor = new FileCommandProcessorAdapter{fileName, logObserver};
			if (dynamic_cast<FileCommandProcessorAdapter*>(cmdProcessor)->getFileLineReader().getFile()) {
				isValid = true;
			} else {
				delete cmdProcessor;
				cmdProcessor = new CommandProcessor{logObserver};
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
					mapLoader = new MapLoader{cmdString};
					cmd.saveEffect("\"" + cmdString + "\" loaded successfully.");
					transition(cmd);
				} else {
					cmd.saveEffect("Invalid file name.");
				}
				file.close();
			} else if (cmdString == "validatemap") {
				if (mapLoader->getMap().validate()) {
					cmd.saveEffect("Map validated successfully.");
					transition(cmd);
				} else {
					cmd.saveEffect("Invalid Map.");
				}
			} else if (cmdString == "addplayer") {
				std::getline(iss, cmdString);
				cmdString.erase(0, 1);
				if (playersList->size() < 6) {
					playersList->emplace_back(Player{cmdString, logObserver});
					cmd.saveEffect("Player \"" + cmdString + "\" added.");
					transition(cmd);
				} else {
					cmd.saveEffect("Player limit reached.");
				}
			} else if (cmdString == "gamestart") {
				if (playersList->size() > 1) {
					size_t numOfPlayerTerritories{mapLoader->getMap().getGameMap().size() / 10 - mapLoader->getMap().getGameMap().size() / 2 % playersList->size()};
					std::cout << numOfPlayerTerritories << std::endl;
					std::vector<int> territoryDistribution;
					for (size_t i = 0; i < numOfPlayerTerritories; i++) {
						territoryDistribution.emplace_back(i % playersList->size());
					}

					std::shuffle(playersList->begin(), playersList->end(), std::default_random_engine(std::time(0)));
					std::shuffle(territoryDistribution.begin(), territoryDistribution.end(), std::default_random_engine(std::time(0)));

					int count{0};
					for (const auto& territory : mapLoader->getMap().getGameMap()) {
						if (count < numOfPlayerTerritories) {
							playersList->at(territoryDistribution.at(count)).addTerritory(territory.first);
							territory.first->setOwner(playersList->at(territoryDistribution.at(count)).getPlayerName());
							count++;
						} else {
							break;
						}
					}
					for (Player& p : *playersList) {
						playerLookup->emplace(p.getPlayerName(), &p);
						p.setReinforcementPool(50);
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
	cmdProcessor = new CommandProcessor{logObserver};
}

// The main game loop.
// The game loops through these three phases until it's done.
void GameEngine::mainGameLoop() {
	GameState& gs{getGameState()};
	while (gs != GameState::Win) {
		reinforcementPhase();
		issueOrdersPhase();
		executeOrdersPhase();
	}
}

// This is where players get their reinforcements.
// The reinforcements they'll get is the number of territories divided by 3, with a minimum of 3.
// They also get extra reinforcements if they own a complete continent depending on the continent's bonus value.
void GameEngine::reinforcementPhase() {
	GameState& gs{getGameState()};
	std::cout << "Reinforcement Phase\n" << std::endl;

	int reinforcements;
	for (Player& p : *playersList) {
		reinforcements = p.getPlayerTerritories().size() / 3;
		if (reinforcements <= 3) {
			p.addReinforcements(3);
		} else {
			p.addReinforcements(reinforcements);
		}
	}

	for (const auto& continent : mapLoader->getMap().getContinents()) {
		std::string owner{mapLoader->getMap().getGameMap().find(*continent.second.begin())->first->getOwner()};
		if (owner != "Neutral") {
			bool allSameOwner{true};
			for (const auto& territory : continent.second) {
				if (owner != mapLoader->getMap().getGameMap().find(territory)->first->getOwner()) {
					allSameOwner = false;
					break;
				}
			}

			if (allSameOwner) {
				std::cout << "Continent of " << continent.first << " is fully owned by " << owner << std::endl;
				std::cout << owner << " receives additional reinforcements of " << mapLoader->getMap().getContinentBonuses().at(continent.first) << std::endl;
				for (Player& player : *playersList) {
					if (owner == player.getPlayerName()) {
						player.addReinforcements(mapLoader->getMap().getContinentBonuses().at(continent.first));
					}
				}
			}
		}
	}

	for (const Player& p : *playersList) {
		std::cout << p << std::endl;
	}
	
	Command& cmd{cmdProcessor->getCommand("issueorder")};
	if (cmdProcessor->validate(cmd, gs)) {
		transition(cmd);
	}
}

// This is where players issue all their orders in round-robin fashion until each player has no more orders to give
void GameEngine::issueOrdersPhase() {
	GameState& gs{getGameState()};
	std::cout << "Issue Orders Phase\n" << std::endl;
	std::vector<bool> finishedOrders(playersList->size(), false);
	int endCount{0};

	while (endCount < playersList->size()) {
		for (size_t i = 0; i < playersList->size(); i++) {
			if (finishedOrders.at(i) == false) {
				playersList->at(i).issueOrder(*cmdProcessor, *deck);
				if (cmdProcessor->getCommandList().back().getCommand() == "done") {
					finishedOrders.at(i) = true;
					endCount++;
				}
			}
		}
	}
	std::cout << std::endl;
	
	Command& cmd{cmdProcessor->getCommand("endissueorders")};
	if (cmdProcessor->validate(cmd, gs)) {
		transition(cmd);
	}
}

// This is where the players' orders get validated and executed in round-robin fashion
// However, it will first go through all deploy orders from all players first
void GameEngine::executeOrdersPhase() {
	GameState& gs{getGameState()};
	std::cout << "Execute Orders Phase\n" << std::endl;

	for (auto& p : *playersList) {
		for (size_t j = 0; j < p.getPlayerOrdersList().getOrders().size(); j++) {
			Order* o = p.getPlayerOrdersList().getOrders().at(j);
			if (typeid(*o) == typeid(Deploy)) {
				o->execute(*this, p);
				std::cout << p.getPlayerName() << std::endl;
				std::cout << *o << std::endl;
				p.getPlayerOrdersList().remove(j);
				j--;
			}
		}
	}

	std::vector<bool> finishedOrders(playersList->size(), false);
	int endCount{0};
	while (endCount < playersList->size()) {
		for (size_t i = 0; i < playersList->size(); i++) {
			Player& p{playersList->at(i)};
			if (finishedOrders.at(i) == false) {
				if (p.getPlayerOrdersList().getOrders().size() == 0) {
					finishedOrders.at(i) = true;
					endCount++;
				} else {
					Order* o = p.getPlayerOrdersList().getOrders().at(0);
					o->execute(*this, p);
					std::cout << p.getPlayerName() << std::endl;
					std::cout << *o << std::endl;
					p.getPlayerOrdersList().remove(0);
				}
			}
		}
	}
	for (size_t i = 0; i < playersList->size(); i++) {
		Player& p{playersList->at(i)};
		if (p.getDrawCard()) {
			deck->draw(p.getPlayerHand());
			p.getDrawCard() = false;
		}
		if (p.getPlayerTerritories().empty()) {
			std::cout << p.getPlayerName() << " has been eliminated!" << std::endl;
			playersList->erase(playersList->begin() + i);
			i--;
		}
	}
	
	if (playersList->size() == 1) {
		std::cout << playersList->at(0).getPlayerName() << " is the last remaining player." << std::endl; 
		std::cout << playersList->at(0).getPlayerName() << " is the WINNER!!!!!" << std::endl;
		Command& cmd{cmdProcessor->getCommand("win")};
		if (cmdProcessor->validate(cmd, gs)) {
			transition(cmd);
		}
	} else {
		Command& cmd{cmdProcessor->getCommand("endexecorders")};
		if (cmdProcessor->validate(cmd, gs)) {
			transition(cmd);
		}
	}	
}

std::string GameEngine::stringToLog() const {
	return gameStateToString();
}
