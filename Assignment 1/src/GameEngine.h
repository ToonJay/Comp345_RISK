#ifndef _GAMEENGINE_H_
#define _GAMEENGINE_H_
#include <iostream>
#include <string>

enum class GameState {Start, Map_Loaded, Map_Validated, Players_Added, Assign_Reinforcement, Issue_Orders, Execute_Orders, Win};

/*
* GameEngine class
* Controls the state of the game with states, transitions and commands.
*/
class GameEngine {
private:
	GameState* gameState;
	std::string command;
public:
	// Constructors
	// Default constructor
	GameEngine();
	// Copy constructor
	GameEngine(const GameEngine& source);
	// Destructor
	~GameEngine();

	// Operator overloads
	// Copy assignment operator overload
	GameEngine& operator=(const GameEngine& rhs);
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const GameEngine& obj);

	// Getter
	GameState& GetGameState() const;

	// Starts and controls the game loop
	void GameLoop();
};

#endif