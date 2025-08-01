#include "CommandProcessing.h"
#include "GameEngine.h"
#include <sstream>

void testCommandProcessor() {
	std::cout << "Please choose to get commands from console or file <filename>: ";
	std::string input;
	std::getline(std::cin, input);
	std::cout << std::endl;
	std::istringstream iss{input};
	std::string command;
	iss >> command;
	if (command == "console") {
		GameEngine ge;
		CommandProcessor& cmdprocessor{ge.getCommandProcessor()};
		while (ge.getGameState() != GameState::Exit_Program) {
			std::cout << "Current Game State: " << ge << std::endl;
			std::cout << "Please enter a command: ";
			Command& c = cmdprocessor.getCommand();
			if (cmdprocessor.validate(c, ge.getGameState())) {
				ge.transition(c);
			};
			std::cout << c << std::endl;
		}
	} else if (command == "file") {
		std::string fileName;
		iss >> fileName;
		GameEngine ge;
		FileCommandProcessorAdapter adapter{fileName, &ge.getLogObserver()};
		if (adapter.getFileLineReader().getFile()) {
			while (ge.getGameState() != GameState::Exit_Program) {
				std::cout << "Current Game State: " << ge << std::endl;
				std::cout << "Please enter a command: ";
				Command& c = adapter.getCommand();
				if (adapter.validate(c, ge.getGameState())) {
					ge.transition(c);
				};
				std::cout << std::endl << c << std::endl;
			}
		} else {
			std::cout << "Invalid file name." << std::endl;
		}
	}
}