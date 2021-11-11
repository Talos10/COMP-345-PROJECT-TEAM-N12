#include "log_observer.h"

#define LOG_DIRECTORY "../log/"


// Default constructor
LogObserver::LogObserver() {
    os.open(LOG_DIRECTORY + filename);

    this->_listOfSubjects = new list<Subject*>();
}



// Parametrized constructor
LogObserver::LogObserver(vector<Subject*> *subjectsList) {
    os.open(LOG_DIRECTORY + filename);

    this->_listOfSubjects = new list<Subject*>(subjectsList->begin(), subjectsList->end());
    for(Subject* s : *_listOfSubjects) {
        s->Attach(this);
    }
}

// Destructor
LogObserver::~LogObserver() {
    os.close();
    for(Subject* s : *_listOfSubjects) {
        s->Detach(this);
        delete s;
    }
    delete _listOfSubjects;
    _listOfSubjects = nullptr;
}

// Adding a subject to the list of subjects
void LogObserver::AddSubject(Subject &subject) {
    _listOfSubjects->push_back(&subject);
    subject.Attach(this);
}

// Removing a subject to the list of subjects
void LogObserver::RemoveSubject(Subject &subject) {
    _listOfSubjects->remove(&subject);
    subject.Detach(this);
}

void LogObserver::Update(const ILoggable& log) {
    string text = log.stringToLog();
    writeLog(text);
}

void LogObserver::writeLog(const string& text) {
    os << text << endl;
    cout << text << endl;
}

list<Subject*>* LogObserver::getAllSubjects() {
    return this->_listOfSubjects;
}