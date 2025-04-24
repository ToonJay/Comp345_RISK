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
	std::cout << "Command Destructor called." << std::endl;
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

// Gets command from the console as a string
void CommandProcessor::readCommand() {
	std::string command;
	std::getline(std::cin, command);
	saveCommand(command);
}

// Stores command inside a list of Command objects
void CommandProcessor::saveCommand(std::string& command) {
	Command c{command};
	commandList->emplace_back(c);
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
	std::cout << "CommandProcessor Destructor called." << std::endl;
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

// Getter
std::list<Command>& CommandProcessor::getCommandList() const {
	return *commandList;
}

// Creates, stores and returns a Command object
// Public method for GameEngine and Player to be able to get a Command object
Command& CommandProcessor::getCommand() {
	readCommand();
	return getCommandList().back();
}

// Validates whether or not a command is valid during the current game state
void CommandProcessor::validate(Command& command, GameState& gameState) const {
	switch (gameState) {
	case GameState::Start:
		if (command.getCommand() == "loadmap <mapfile>") {
			gameState = GameState::Map_Loaded;
			command.saveEffect("Map loaded.");
		} else {
			command.saveEffect("Invalid command during Start. Please enter \"loadmap <mapfile>\".");
		}
		break;
	case GameState::Map_Loaded:
		if (command.getCommand() == "loadmap <mapfile>") {
			command.saveEffect("Map loaded.");
		} else if (command.getCommand() == "validatemap") {
			gameState = GameState::Map_Validated;
			command.saveEffect("Map validated.");
		} else {
			command.saveEffect("Invalid command during Map_Loaded. Please enter \"loadmap <mapfile>\" or \"validatemap\".");
		}
		break;
	case GameState::Map_Validated:
		if (command.getCommand() == "addplayer <playername>") {
			gameState = GameState::Players_Added;
			command.saveEffect("Player added.");
		} else {
			command.saveEffect("Invalid command during Map_Validated. Please enter \"addplayer\".");
		}
		break;
	case GameState::Players_Added:
		if (command.getCommand() == "addplayer <playername>") {
			command.saveEffect("Player added.");
		} else if (command.getCommand() == "gamestart") {
			gameState = GameState::Assign_Reinforcement;
			command.saveEffect("Game Start!");
		} else {
			command.saveEffect("Invalid command during Players_Added. Please enter \"addplayer\" or \"assigncountries\".");
		}
		break;
	case GameState::Assign_Reinforcement:
		if (command.getCommand() == "issueorder") {
			gameState = GameState::Issue_Orders;
			command.saveEffect("Order issued.");
		} else {
			command.saveEffect("Invalid command during Assign_Reinforcement. Please enter \"issueorder\".");
		}
		break;
	case GameState::Issue_Orders:
		if (command.getCommand() == "issueorder") {
			command.saveEffect("Order issued.");
		} else if (command.getCommand() == "endissueorders") {
			gameState = GameState::Execute_Orders;
			command.saveEffect("Finished issueing orders.");
		} else {
			command.saveEffect("Invalid command during Issue_Orders. Please enter \"issueorder\" or \"endissueorders\".");
		}
		break;
	case GameState::Execute_Orders:
		if (command.getCommand() == "execorder") {
			command.saveEffect("Order executed.");
		} else if (command.getCommand() == "endexecorders") {
			gameState = GameState::Assign_Reinforcement;
			command.saveEffect("Finished executing orders.");
		} else if (command.getCommand() == "win") {
			gameState = GameState::Win;
			command.saveEffect("END OF GAME!");
		} else {
			command.saveEffect("Invalid command during Execute_Orders. Please enter \"execorder\", \"endexecorders\" or \"win\".");
		}
		break;
	case GameState::Win:
		if (command.getCommand() == "replay") {
			gameState = GameState::Start;
			command.saveEffect("Starting game.");
		} else if (command.getCommand() == "quit") {
			gameState = GameState::Exit_Program;
			command.saveEffect("Exiting game.");
		} else {
			command.saveEffect("Invalid command during Win. Please enter \"replay\" or \"quit\".");
		}
		break;
	default:
		std::cout << "Invalid command." << std::endl;
	}
}

//////////////////////////////////////////////////
// FileLineReader class function definitions
//////////////////////////////////////////////////

// Parameterized constructor
FileLineReader::FileLineReader(std::string fileName) 
	: fileName{new std::string{fileName}}, file{new std::ifstream{fileName}}, line{new std::string} {
}

// Copy constructor
FileLineReader::FileLineReader(const FileLineReader& source) 
	: fileName{new std::string{*source.fileName}}, file{new std::ifstream{*source.fileName}}, line {new std::string{*source.line}} {
}

// Destructor, deletes pointer data members and closes file
FileLineReader::~FileLineReader() {
	delete fileName;
	file->close();
	delete file;
	delete line;
	std::cout << "FineLineReader Destructor called." << std::endl;
}

// Copy assignment operator overload
FileLineReader& FileLineReader::operator=(const FileLineReader& rhs) {
	if (this != &rhs) {
		*fileName = *rhs.fileName;
		file = new std::ifstream{*rhs.fileName};
		*line = *rhs.line;
	}
	return *this;
}

// Stream insertion operator overload
std::ostream& operator<<(std::ostream& os, const FileLineReader& obj) {
	os << "File Name: " << obj.getFileName() << std::endl;
	os << "Current line: " << obj.getLine() << std::endl;
	return os;
}

// Getters
std::string& FileLineReader::getFileName() const {
	return *fileName;
}

std::ifstream& FileLineReader::getFile() const {
	return *file;
}

std::string& FileLineReader::getLine() const {
	return *line;
}

// read line from file
void FileLineReader::readLineFromFile() {
	std::getline(getFile(), getLine());
}

//////////////////////////////////////////////////
// FileCommandProcessorAdapter class function definitions
//////////////////////////////////////////////////

// read command from file
void FileCommandProcessorAdapter::readCommand() {
	flr->readLineFromFile();
	saveCommand(flr->getLine());
}

// Parameterized constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter(std::string file) 
	: flr{new FileLineReader{file}} {
}

// Copy constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter& source) 
	: flr{new FileLineReader{*source.flr}} {
}

// Destructor, deletes pointer data member
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
	delete flr;
	std::cout << "FCPA Destructor called." << std::endl;
}

// Copy assignment operator overload
FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter& rhs) {
	if (this != &rhs) {
		*flr = *rhs.flr;
	}
	return *this;
}

// Stream insertion operator overload
std::ostream& operator<<(std::ostream& os, const FileCommandProcessorAdapter& obj) {
	os << obj.getFileLineReader();
	os << "List of commands: " << std::endl;
	for (const Command& c : obj.getCommandList()) {
		os << c << std::endl;
	}
	return os;
}

// Getter
FileLineReader& FileCommandProcessorAdapter::getFileLineReader() const {
	return *flr;
}