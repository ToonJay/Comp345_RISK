#ifndef _GAMEENGINE_H_
#define _GAMEENGINE_H_
#include <iostream>
#include <string>
#include <algorithm>
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
	Map* map;
	Deck* deck;
	std::vector<Player>* playersList;
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

	// Getters //
	GameState& getGameState() const;
	CommandProcessor& getCommandProcessor() const;

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