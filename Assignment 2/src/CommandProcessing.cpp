#include "CommandProcessing.h"
#include "GameEngine.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Command class function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Parameterized constructor
Command::Command(std::string command) 
	: command{new std::string{command}}, effect{new std::string} {
	//std::cout << "Called Command parameterized constructor" << std::endl;
}

// Copy constructor
Command::Command(const Command& source) 
	: command{new std::string{*source.command}}, effect{new std::string{*source.effect}} {
	//std::cout << "Called Command copy constructor" << std::endl;
}

// Destructor, deletes all pointer data members
Command::~Command() {
	delete command;
	delete effect;
	//std::cout << "Called Command destructor" << std::endl;
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
const std::string& Command::getCommand() const {
	return *command;
}

const std::string& Command::getEffect() const {
	return *effect;
}

// Store effect of an executed command as a string
// Assign a string reference parameter to the object's effect data member
void Command::saveEffect(const std::string& effect) {
	*this->effect = effect;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CommandProcessor class function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Gets command from the console as a string
void CommandProcessor::readCommand() {
	std::string command;
	std::getline(std::cin, command);
	saveCommand(command);
}

// Stores command inside a list of Command objects
void CommandProcessor::saveCommand(const std::string& command) {
	commandList->emplace_back(command);
}

// Default constructor
CommandProcessor::CommandProcessor()
	: commandList{new std::list<Command>} {
	//std::cout << "Called CommandProcessor default constructor" << std::endl;
}

// Copy constructor
CommandProcessor::CommandProcessor(const CommandProcessor& source) 
	: commandList{new std::list<Command>{*source.commandList}} {
	//std::cout << "Called CommandProcessor copy constructor" << std::endl;
}

// Destructor, deallocates the memory for the pointer data member
CommandProcessor::~CommandProcessor() {
	delete commandList;
	//std::cout << "Called CommandProcessor destructor" << std::endl;
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
const std::list<Command>& CommandProcessor::getCommandList() const {
	return *commandList;
}

// Creates, stores and returns a Command object
// Public method for GameEngine and Player to be able to get a Command object
Command& CommandProcessor::getCommand() {
	readCommand();
	return commandList->back();
}

// Validates whether or not a command is valid during the current game state
bool CommandProcessor::validate(Command& command, GameState& gameState) const {
	switch (gameState) {
	case GameState::Start:
		if (command.getCommand().substr(0, 7) == "loadmap") {
			return true;
		} else {
			command.saveEffect("Invalid command during Start. Please enter \"loadmap <mapfile>\".");
			return false;
		}
		break;
	case GameState::Map_Loaded:
		if (command.getCommand().substr(0, 7) == "loadmap") {
			return true;
		} else if (command.getCommand() == "validatemap") {
			return true;
		} else {
			command.saveEffect("Invalid command during Map_Loaded. Please enter \"loadmap <mapfile>\" or \"validatemap\".");
			return false;
		}
		break;
	case GameState::Map_Validated:
		if (command.getCommand().substr(0, 9) == "addplayer") {
			return true;
		} else {
			command.saveEffect("Invalid command during Map_Validated. Please enter \"addplayer\".");
			return false;
		}
		break;
	case GameState::Players_Added:
		if (command.getCommand().substr(0, 9) == "addplayer") {
			return true;
		} else if (command.getCommand() == "gamestart") {
			return true;
		} else {
			command.saveEffect("Invalid command during Players_Added. Please enter \"addplayer\" or \"gamestart\".");
			return false;
		}
		break;
	case GameState::Assign_Reinforcement:
		if (command.getCommand() == "issueorder") {
			return true;
		} else {
			command.saveEffect("Invalid command during Assign_Reinforcement. Please enter \"issueorder\".");
			return false;
		}
	case GameState::Issue_Orders:
		if (command.getCommand() == "issueorder") {
			return true;
		} else if (command.getCommand() == "endissueorders") {
			return true;
		} else {
			command.saveEffect("Invalid command during Issue_Orders. Please enter \"issueorder\" or \"endissueorders\".");
			return false;
		}
	case GameState::Execute_Orders:
		if (command.getCommand() == "execorder") {
			return true;
		} else if (command.getCommand() == "endexecorders") {
			return true;
		} else if (command.getCommand() == "win") {
			return true;
		} else {
			command.saveEffect("Invalid command during Execute_Orders. Please enter \"execorder\", \"endexecorders\" or \"win\".");
			return false;
		}
	case GameState::Win:
		if (command.getCommand() == "replay") {
			return true;
		} else if (command.getCommand() == "quit") {
			return true;
		} else {
			command.saveEffect("Invalid command during Win. Please enter \"replay\" or \"quit\".");
			return false;
		}
	default:
		std::cout << "Invalid command." << std::endl;
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FileLineReader class function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Parameterized constructor
FileLineReader::FileLineReader(std::string fileName) 
	: fileName{new std::string{fileName}}, file{new std::ifstream{fileName}}, line{new std::string} {
	//std::cout << "Called FileLineReader parameterized constructor" << std::endl;
}

// Copy constructor
FileLineReader::FileLineReader(const FileLineReader& source) 
	: fileName{new std::string{*source.fileName}}, file{new std::ifstream{*source.fileName}}, line {new std::string{*source.line}} {
	//std::cout << "Called FileLineReader copy constructor" << std::endl;
}

// Destructor, deletes pointer data members and closes file
FileLineReader::~FileLineReader() {
	delete fileName;
	file->close();
	delete file;
	delete line;
	// std::cout << "Called FileLineReader destructor" << std::endl;
}

// Copy assignment operator overload
FileLineReader& FileLineReader::operator=(const FileLineReader& rhs) {
	if (this != &rhs) {
		*fileName = *rhs.fileName;
		delete file;
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
std::ifstream& FileLineReader::getFile() {
	return *file;
}

std::string& FileLineReader::getLine() {
	return *line;
}

const std::string& FileLineReader::getFileName() const {
	return *fileName;
}

const std::ifstream& FileLineReader::getFile() const {
	return *file;
}

const std::string& FileLineReader::getLine() const {
	return *line;
}

// read line from file
void FileLineReader::readLineFromFile() {
	std::getline(*file, *line);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FileCommandProcessorAdapter class function definitions
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// read command from file
void FileCommandProcessorAdapter::readCommand() {
	flr->readLineFromFile();
	saveCommand(flr->getLine());
}

// Parameterized constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter(std::string file) 
	: flr{new FileLineReader{file}} {
	// std::cout << "Called FileCommandProcessorAdapter parameterized constructor" << std::endl;
}

// Copy constructor
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter& source) 
	: flr{new FileLineReader{*source.flr}} {
	// std::cout << "Called FileCommandProcessorAdapter copy constructor" << std::endl;
}

// Destructor, deletes pointer data member
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
	delete flr;
	// std::cout << "Called FileCommandProcessorAdapter destructor" << std::endl;
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
const FileLineReader& FileCommandProcessorAdapter::getFileLineReader() const {
	return *flr;
}