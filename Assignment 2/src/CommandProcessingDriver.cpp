#include "CommandProcessing.h"
#include "GameEngine.h"
#include <sstream>

std::string enumToString(const GameState gs) {
	switch (gs) {
	case GameState::Start: return "Start";
	case GameState::Map_Loaded: return "Map Loaded";
	case GameState::Map_Validated: return "Map Validated";
	case GameState::Players_Added: return "Players Added";
	case GameState::Assign_Reinforcement: "Assign Reinforcement";
	case GameState::Issue_Orders: return "Issue Orders";
	case GameState::Execute_Orders: return "Execute Orders";
	case GameState::Win: return "Win";
	default: return "Error";
	}
}

void testCommandProcessor() {
	std::cout << "Please choose to get commands from console or file <filename>: ";
	std::string input;
	std::getline(std::cin, input);
	std::cout << std::endl;
	std::istringstream iss{input};
	std::string command;
	iss >> command;
	if (command == "console") {
		GameState gs{GameState::Start};
		CommandProcessor cmdprocessor{};
		while (gs != GameState::Exit_Program) {
			std::cout << "Current Game State: " << enumToString(gs) << std::endl;
			std::cout << "Please enter a command: ";
			Command& c = cmdprocessor.getCommand();
			cmdprocessor.validate(c, gs);
			std::cout << c << std::endl;
		}
	} else if (command == "file") {
		std::string fileName;
		iss >> fileName;
		GameState gs{GameState::Start};
		FileCommandProcessorAdapter adapter{fileName};
		if (adapter.getFileLineReader().getFile()) {
			while (gs != GameState::Exit_Program) {
				std::cout << "Current Game State: " << enumToString(gs) << std::endl;
				std::cout << "Please enter a command: ";
				Command& c = adapter.getCommand();
				adapter.validate(c, gs);
				std::cout << std::endl << c << std::endl;
			}
		} else {
			std::cout << "Invalid file name." << std::endl;
		}
	}
}