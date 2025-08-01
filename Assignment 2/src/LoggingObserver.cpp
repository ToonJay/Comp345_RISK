#include "LoggingObserver.h"

void Subject::attach(Observer& observer) {
	observers.push_back(&observer);
}

void Subject::detach(Observer& observer) {
	observers.erase(
		std::remove(observers.begin(), observers.end(), &observer),
		observers.end()
	);
}

void Subject::notify(const ILoggable& loggable) {
	for (Observer* obs : observers) {
		obs->update(loggable);
	}
}

void LogObserver::update(const ILoggable& loggable) {
	std::ofstream logFile("./src/gamelog.txt", std::ios::app);
	if (logFile.is_open()) {
		logFile << loggable.stringToLog() << std::endl;
	}
	logFile.close();
}
