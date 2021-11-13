#pragma once

#include "observer/logging_observer.h"
#include "orders/orders.h"
#include <fstream>
#include <vector>


class LogObserver : public Observer {
private:
    // Declare a pointer to a list of pointers to subjects
    list<Subject*>* _listOfSubjects;

    ofstream os;
public:
    // Default constructor
    LogObserver();

    // Parametrized constructor
    LogObserver(vector<Subject*>* subjectsList);

    // Copy constructor
    LogObserver(const LogObserver& log);

    // Assignment operator
    LogObserver& operator=(const LogObserver& logObserver);

    // Defining the output operator
    friend ostream& operator << (ostream& out, const LogObserver& lo);

    // Destructor
    ~LogObserver();

    // Add a subject
    void AddSubject(Subject& subject);

    // Remove a subject
    void RemoveSubject(Subject& subject);

    // This method calls the writeLog method to update the text file
    void Update(const ILoggable& log) override;

    // This method writes the corresponding text to the text file
    void writeLog(const string& text);

    // Get all subjects associated to the log obserber
    list<Subject*>* getAllSubjects() const;
};

static string filename = "gamelog.txt";