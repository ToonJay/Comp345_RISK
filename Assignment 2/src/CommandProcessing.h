#ifndef _COMMANDPROCESSING_H_
#define _COMMANDPROCESSING_H_
#include <iostream>
#include <string>
#include <list>

enum class GameState;

/*
* Command class
* Stores a command and its effect as strings
*/
class Command {
private:
	std::string* command;
	std::string* effect;
public:
	// Constructors
	// Parameterized constructor
	Command(std::string command);
	// Copy constructor
	Command(const Command& source);
	// Destructor
	~Command();

	// Operator overloads
	// Copy assignment operator overload
	Command& operator=(const Command& rhs);
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const Command& obj);

	// Getters
	std::string& getCommand() const;
	std::string& getEffect() const;

	// Store effect of an executed command as a string
	void saveEffect(const std::string& effect);
};

/*
* CommandProcessor class
* Class used to create, store and validate console commands.
*/
class CommandProcessor {
private:
	std::list<Command>* commandList;
	void readCommand();
	void saveCommand(std::string& command);
public:
	// Constructors
	// Default constructor
	CommandProcessor();
	// Copy constructor
	CommandProcessor(const CommandProcessor& source);
	// Destructor
	~CommandProcessor();

	// Operator overloads
	// Copy assignment operator overload
	CommandProcessor& operator=(const CommandProcessor& rhs);
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const CommandProcessor& obj);

	// Getters
	std::list<Command>& getCommandList() const;
	Command& getCommand() const;

	// Validates whether or not a command is valid during the current game state
	bool validate(const Command& command, GameState& gameState) const;
};

#endif