#include "CommandProcessing.h"
#include "GameEngine.h"

//////////////////////////////////////////////////
// Command class function definitions
//////////////////////////////////////////////////

// Parameterized constructor
Command::Command(std::string command) 
	: command{new std::string{command}}, effect{new std::string} {
}

// Copy constructor
Command::Command(const Command& source) 
	: command{new std::string{*source.command}}, effect{new std::string{*source.effect}} {
}

// Destructor, deletes all pointer data members
Command::~Command() {
	delete command;
	delete effect;
}

// Copy assignment operator overload
Command& Command::operator=(const Command& rhs) {
	if (this != &rhs) {
		*command = *rhs.command;
		*effect = *rhs.effect;
	}
	return *this;
}

// Stream insertion operator overload
std::ostream& operator<<(std::ostream& os, const Command& obj) {
	os << "Command: " << obj.getCommand() << std::endl;
	if (!obj.getEffect().empty()) {
		os << "Effect: " << obj.getEffect() << std::endl;
	}
	return os;
}

// Getters
std::string& Command::getCommand() const {
	return *command;
}

std::string& Command::getEffect() const {
	return *effect;
}

// Store effect of an executed command as a string
// Assign a string reference parameter to the object's effect data member
void Command::saveEffect(const std::string& effect) {
	*this->effect = effect;
}

//////////////////////////////////////////////////
// CommandProcessor class function definitions
//////////////////////////////////////////////////

void CommandProcessor::readCommand() {
}

void CommandProcessor::saveCommand(std::string& command) {
}

// Default constructor
CommandProcessor::CommandProcessor()
	: commandList{new std::list<Command>} {
}

// Copy constructor
CommandProcessor::CommandProcessor(const CommandProcessor& source) 
	: commandList{new std::list<Command>{*source.commandList}} {
}

// Destructor, deallocates the memory for the pointer data member
CommandProcessor::~CommandProcessor() {
	delete commandList;
}

// Copy assignment operator overload
CommandProcessor& CommandProcessor::operator=(const CommandProcessor& rhs) {
	if (this != &rhs) {
		*commandList = *rhs.commandList;
	}
	return *this;
}

// Stream insertion operator overload
std::ostream& operator<<(std::ostream& os, const CommandProcessor& obj) {
	os << "List of commands: " << std::endl;
	for (const Command& c : obj.getCommandList()) {
		os << c << std::endl;
	}
	return os;
}

// Getters
std::list<Command>& CommandProcessor::getCommandList() const {
	return *commandList;
}

Command& CommandProcessor::getCommand() const {
}

bool CommandProcessor::validate(const Command& command, GameState& gameState) const {
	return true;
}