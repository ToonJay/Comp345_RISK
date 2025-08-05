#include "LoggingObserver.h"

// Attach observer to subject
void Subject::attach(Observer& observer) {
	observers.push_back(&observer);
}

// Detach observer from subject
void Subject::detach(Observer& observer) {
	observers.erase(
		std::remove(observers.begin(), observers.end(), &observer),
		observers.end()
	);
}

// Notify observer of change to loggable object
void Subject::notify(const ILoggable& loggable) {
	for (Observer* obs : observers) {
		obs->update(loggable);
	}
}

// Update the observer
// Call stringToLog to write into gamelog.txt
void LogObserver::update(const ILoggable& loggable) {
	std::ofstream logFile("./src/gamelog.txt", std::ios::app);
	if (logFile.is_open()) {
		logFile << loggable.stringToLog() << std::endl;
	}
	logFile.close();
}
