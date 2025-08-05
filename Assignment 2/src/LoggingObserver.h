#ifndef _LOGGINGOBSERVER_H
#define _LOGGINGOBSERVER_H

#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IMPORTANT NOTE : I'm breaking the rules of having all data members be pointers,
// as well as all classes having a copy constructor, assignment operator and stream insertion operator
// since it seems more important to follow design principles of a design pattern, which is the observer pattern in this case.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
* ILoggable Interface
*/
class ILoggable {
public:
    // Return string to write to log
    virtual std::string stringToLog() const = 0;
    virtual ~ILoggable() = default;
};

/*
* Observer Interface
*/
class Observer {
public:
    virtual void update(const ILoggable& loggable) = 0;
    virtual ~Observer() = default;
};

/*
* Subject Base Class
*/
class Subject {
private:
    std::vector<Observer*> observers;

public:
    virtual ~Subject() = default;

    // Attach observer to subject
    void attach(Observer& observer);
    // Detach observer from subject
    void detach(Observer& observer);
    // Notify observer of change to loggable object
    void notify(const ILoggable& loggable);
};

/*
* LogObserver Concrete Class
*/
class LogObserver : public Observer {
public:
    // Update the observer
    void update(const ILoggable& loggable) override;
};

#endif