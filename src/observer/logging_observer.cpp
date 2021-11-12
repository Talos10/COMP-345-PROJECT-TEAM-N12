#include "logging_observer.h"

/////////////////////// Observer class ///////////////////////
Observer::Observer() = default;

Observer::~Observer() = default;

/////////////////////// Subject class ///////////////////////
Subject::Subject() {
    _observers = new list<Observer *>;
}

Subject::~Subject() {
    delete _observers;
}

void Subject::Attach(Observer* o) {
    _observers->push_back(o);
}

void Subject::Detach(Observer *o) {
    _observers->remove(o);
}

void Subject::Notify(const ILoggable& log) {
    list<Observer *>::iterator i = _observers->begin();
    for (; i != _observers->end(); ++i)
        (*i)->Update(log);
}

list<Observer *> *Subject::getAllObservers() const {
    return this->_observers;
}

/////////////////////// ILoggable class ///////////////////////
ILoggable::ILoggable() = default;

ILoggable::~ILoggable() = default;
