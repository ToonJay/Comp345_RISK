#ifndef _COMMANDPROCESSING_H_
#define _COMMANDPROCESSING_H_
#include <iostream>
#include <string>
#include <list>
#include <fstream>
#include "LoggingObserver.h"

enum class GameState;

/*
* Command class
* Stores a command and its effect as strings
*/
class Command : public Subject, public ILoggable {
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

	//--Getters--//
	const std::string& getCommand() const;
	const std::string& getEffect() const;

	// Store effect of an executed command as a string
	void saveEffect(const std::string& effect);

	// Logs command effect
	virtual std::string stringToLog() const override;
};

/*
* CommandProcessor class
* Class used to create, store and validate console commands.
*/
class CommandProcessor : public Subject, public ILoggable {
private:
	std::list<Command>* commandList;
	LogObserver* logObserver;
	// read command from console and store into a string
	virtual void readCommand();
protected:
	// save command as a Command object
	void saveCommand(const std::string& command);
public:
	//--Constructors--//
	// Default constructor
	CommandProcessor();
	// Parameterized constructor
	CommandProcessor(LogObserver* logObserver);
	// Copy constructor
	CommandProcessor(const CommandProcessor& source);
	// Destructor
	virtual ~CommandProcessor();

	//--Operator overloads--//
	// Copy assignment operator overload
	CommandProcessor& operator=(const CommandProcessor& rhs);
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const CommandProcessor& obj);

	//--Getters--//
	const std::list<Command>& getCommandList() const;
	LogObserver& getLogObserver();
	const LogObserver& getLogObserver() const;

	//--Setter--//
	void setLogObserver(LogObserver* logObserver);

	// Creates, stores and returns Command object
	Command& getCommand();
	// Creates, stores and returns Command object using a string parameter instead of the console
	Command& getCommand(const std::string& command);

	// Validates whether or not a command is valid during the current game state
	bool validate(Command& command, GameState& gameState) const;
	// Logs last command's description
	virtual std::string stringToLog() const override;
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

	//--Getters--//
	std::ifstream& getFile();
	std::string& getLine();
	const std::string& getFileName() const;
	const std::ifstream& getFile() const;
	const std::string& getLine() const;

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
	virtual void readCommand() override;
public:
	//--Constructors--//
	// Parameterized constructor
	FileCommandProcessorAdapter(std::string file, LogObserver* logObserver);
	// Copy constructor
	FileCommandProcessorAdapter(const FileCommandProcessorAdapter& source);
	// Destructor
	virtual ~FileCommandProcessorAdapter();

	//--Operator overloads--//
	// Copy assignment operator overload
	FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& rhs);
	// Stream insertion operator overload
	friend std::ostream& operator<<(std::ostream& os, const FileCommandProcessorAdapter& obj);

	//--Getter--//
	const FileLineReader& getFileLineReader() const;
};

#endif