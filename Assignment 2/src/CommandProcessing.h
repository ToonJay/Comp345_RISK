#ifndef _COMMANDPROCESSING_H_
#define _COMMANDPROCESSING_H_
#include <iostream>
#include <string>
#include <list>
#include <fstream>

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
	//--Constructors--//
	// Parameterized constructor
	Command(std::string command);
	// Copy constructor
	Command(const Command& source);
	// Destructor
	~Command();

	//--Operator overloads--//
	// Copy assignment operator overload
	Command& operator=(const Command& rhs);
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const Command& obj);

	// Getters //
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
	// read command from console and store into a string
	virtual void readCommand() const;
protected:
	// save command as a Command object
	void saveCommand(std::string& command) const;
public:
	//--Constructors--//
	// Default constructor
	CommandProcessor();
	// Copy constructor
	CommandProcessor(const CommandProcessor& source);
	// Destructor
	virtual ~CommandProcessor();

	//--Operator overloads--//
	// Copy assignment operator overload
	CommandProcessor& operator=(const CommandProcessor& rhs);
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const CommandProcessor& obj);

	// Getter //
	std::list<Command>& getCommandList() const;

	// Creates, stores and returns Command object
	Command& getCommand() const;

	// Validates whether or not a command is valid during the current game state
	bool validate(Command& command, GameState& gameState) const;
};

/*
* FileLineReader class
* Opens a file and reads lines from it
* Stores the file name, file input stream, and the most recently read line
*/
class FileLineReader {
private:
	std::string* fileName;
	std::ifstream* file;
	std::string* line;
public:
	//--Constructors--//
	// Parameterized constructor
	FileLineReader(std::string fileName);
	// Copy constructor
	FileLineReader(const FileLineReader& source);
	// Destructor
	~FileLineReader();

	//--Operator overloads--//
	// Copy assignment operator overload
	FileLineReader& operator=(const FileLineReader& rhs);
	// Stream insertion overpeator overload
	friend std::ostream& operator<<(std::ostream& os, const FileLineReader& obj);

	// Getters //
	std::string& getFileName() const;
	std::ifstream& getFile() const;
	std::string& getLine() const;

	// read line from file
	void readLineFromFile();
};

/*
* FileCommandProcessorAdapter class
* Allows for commands to be read from a text file instead of on console
* Inherits from CommandProcessor and contains/uses a FileLineReader
*/
class FileCommandProcessorAdapter : public CommandProcessor {
private:
	FileLineReader* flr;
	// read command from file
	virtual void readCommand() const override;
public:
	//--Constructors--//
	// Parameterized constructor
	FileCommandProcessorAdapter(std::string file);
	// Copy constructor
	FileCommandProcessorAdapter(const FileCommandProcessorAdapter& source);
	// Destructor
	virtual ~FileCommandProcessorAdapter();

	//--Operator overloads--//
	// Copy assignment operator overload
	FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& rhs);
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const FileCommandProcessorAdapter& obj);

	// Getter //
	FileLineReader& getFileLineReader() const;
};

#endif