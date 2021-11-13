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

// Copy constructor
LogObserver::LogObserver(const LogObserver &log) {
    list<Subject*>::iterator it;
    for(int i = 0; i < log.getAllSubjects()->size(); i++) {
        it = log.getAllSubjects()->begin();
        advance(it,i);
        Subject* c = new Subject(*(*it));
        this->_listOfSubjects->emplace_back(c);
    }
    this->os.open(LOG_DIRECTORY + filename);
}

// Assignment operator
LogObserver& LogObserver::operator=(const LogObserver& logObserver) {
    if(this != &logObserver){
        // Delete all cards in left hand side in case there are already members
        for(int index = 0; index < this->_listOfSubjects->size(); index++) {
            this->_listOfSubjects->pop_back();
        }
        // add all cards from right hand side to left hand side
        list<Subject*>::iterator it;
        for (int i = 0 ; i < logObserver.getAllSubjects()->size(); i++) {
            it = logObserver.getAllSubjects()->begin();
            advance(it,i);
            Subject* c = new Subject(*(*it));
            this->_listOfSubjects->emplace_back(c);
        }
    }
    this->os.open(LOG_DIRECTORY + filename);
    return *this;
}

// Output operator
ostream& operator << (ostream& out, const LogObserver& lo) {
    out << "The log observer contains " << lo._listOfSubjects->size() << " subjects" << endl;
    return out;
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
}

list<Subject*>* LogObserver::getAllSubjects() const {
    return this->_listOfSubjects;
}