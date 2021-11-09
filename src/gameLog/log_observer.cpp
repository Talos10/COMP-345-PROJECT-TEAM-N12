#include "log_observer.h"

#define LOG_DIRECTORY "../log/"


// Default constructor
LogObserver::LogObserver() {
    //ofstream os;
    os.open(LOG_DIRECTORY + filename);
    os << "constructor" << endl;
    //os.close();

    this->_listOfSubjects = new list<Subject*>();
}



// Parametrized constructor
LogObserver::LogObserver(vector<Subject*> *subjectsList) {
    //ofstream os;
    os.open(LOG_DIRECTORY + filename);
    os << "parametrized constructor" << endl;
    //os.close();

    this->_listOfSubjects = new list<Subject*>(subjectsList->begin(), subjectsList->end());
    for(Subject* s : *_listOfSubjects) {
        s->Attach(this);
    }
}

// Copy constructor
LogObserver::LogObserver(const LogObserver &lo) {
//    for (int i = 0; i < lo._listOfSubjects->size(); i++) {
//        list<Subject*>::iterator it = lo._listOfSubjects->begin();
//        Subject* s = new Subject(*it);
//        this->AddSubject(s);
//    }
}

// Assignment operator
//LogObserver &LogObserver::operator=(const LogObserver &logObserver) {
//
//}

// Output operator
ostream& operator << (ostream& out, const LogObserver& lo) {
//    out << "The subjects list contains " << lo._listOfSubjects->size() << " subjects:" << endl;
//    for(Subject* s: *(lo._listOfSubjects)) {
//        out << *s << endl;
//    }
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
    cout << text << endl;
}

list<Subject*>* LogObserver::getAllSubjects() {
    return this->_listOfSubjects;
}