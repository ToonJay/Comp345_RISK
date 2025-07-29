#ifndef _GAMEENGINE_H_
#define _GAMEENGINE_H_
#include <iostream>
#include <string>
#include <random>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include "Player.h"
#include "Map.h"
#include "Cards.h"
#include "Orders.h"
#include "CommandProcessing.h"

enum class GameState {Start, Map_Loaded, Map_Validated, Players_Added, Assign_Reinforcement, Issue_Orders, Execute_Orders, Win, Exit_Program};

/*
* GameEngine class
* Controls the state of the game with states, transitions and commands.
*/
class GameEngine {
private:
	GameState* gameState;
	CommandProcessor* cmdProcessor;
	MapLoader* mapLoader;
	Deck* deck;
	std::vector<Player>* playersList;
	std::unordered_map<std::string, Player*>* playerLookup;
	std::unordered_map<std::string, std::string>* diplomacyMap;
public:
	//--Constructors--//
	// Default constructor
	GameEngine();
	// Copy constructor
	GameEngine(const GameEngine& source);
	// Destructor
	~GameEngine();

	//--Operator overloads--//
	// Copy assignment operator overload
	GameEngine& operator=(const GameEngine& rhs);
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const GameEngine& obj);

	//--Getters--//
	GameState& getGameState();
	CommandProcessor& getCommandProcessor();
	MapLoader& getMapLoader();
	Deck& getDeck();
	Player& getPlayerByName(const std::string& name);
	std::unordered_map<std::string, std::string>& getDiplomacyMap();
	

	const GameState& getGameState() const;
	const CommandProcessor& getCommandProcessor() const;
	const MapLoader& getMapLoader() const;
	const Deck& getDeck() const;
	const std::unordered_map<std::string, Player*>& getPlayerLookup() const;
	const Player& getPlayerByName(const std::string& name) const;
	const std::unordered_map<std::string, std::string>& getDiplomacyMap() const;
	
	// Transitions from one gameState to the next
	void transition(const Command& command);

	//--Game phases--//
	// Where the game is setup
	void startupPhase();
	// Where the game is played once started. Keeps looping until the end of the game.
	void mainGameLoop();
	void reinforcementPhase();
	void issueOrdersPhase();
	void executeOrdersPhase();
};

#endif