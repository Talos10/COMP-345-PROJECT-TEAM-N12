#pragma once

#include <list>
#include <string>

using namespace std;

class Observer;
class Subject;
class ILoggable;

class Observer {
public:
    Observer();
    ~Observer();
    virtual void Update(const ILoggable& log) = 0;
};

class Subject {
public:
    Subject();
    ~Subject();
    virtual void Attach(Observer* o);
    virtual void Detach(Observer* o);
    virtual void Notify(const ILoggable& log);
    list<Observer*>* getAllObservers() const;
private:
    list<Observer*> *_observers;
};

class ILoggable {
public:
    ILoggable();
    ~ILoggable();
    virtual string stringToLog() const = 0;
};