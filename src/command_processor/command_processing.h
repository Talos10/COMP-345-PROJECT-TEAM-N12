#ifndef COMP_345_PROJECT_TEAM_N12_COMMAND_PROCESSING_H
#define COMP_345_PROJECT_TEAM_N12_COMMAND_PROCESSING_H

#include <deque>
#include <string>
#include <fstream>
#include <game_engine/game_engine.h>

using namespace std;

// Forward declaration.
class GameEngine;

// This class represents an object that represents an action or command from the user which will be validated before
// being executed if valid. These commands dictate the flow of the game.
class Command {
private:
    // A string which holds the entire command entered by the user or read from the file.
    string *commandName;

    // A string which holds the effect of the command on the game (if valid, the effect it had on the game and if invalid,
    // then the reason why the command is invalid).
    string *commandEffect;

    // A list containing all the keywords from the command (i.e. all the words that were separated by spaces).
    vector<string *> *commandArgs;

    // Defining the output operator for the Command object.
    friend std::ostream &operator<<(std::ostream &stream, const Command &cmd);

    // A function used in the assignment operator definition which swaps the member data
    // between two Command objects.
    void swap(Command &first, Command &second);

public:
    // One param constructor which creates a Command object with a string corresponding to the entire command entered by
    // the user or read from the file.
    explicit Command(const string &command);

    // A copy constructor.
    Command(const Command &cmd);

    // Defining the assignment operator.
    Command &operator=(Command command);

    // Destructor.
    ~Command();

    // Getter for the command name (entire command).
    [[nodiscard]] string *getCommandName() const;

    // Getter for the command effect.
    [[nodiscard]] string *getCommandEffect() const;

    // Getter for the command arguments.
    [[nodiscard]] vector<string *> *getCommandArgs() const;

    // Method which saves the effect of the command given a certain effect and given whether the command is valid or not.
    void saveEffect(const string &newEffect, bool isCommandValid);
};

// An object which reads from a file line by line.
class FileLineReader {
private:
    // The path to the file to be read.
    string *filePath;

    // A stream which is used to read from a file (txt file in our case).
    ifstream *file;

    // Defining the output operator for the FileLineReader object.
    friend std::ostream &operator<<(std::ostream &stream, const FileLineReader &cmd);

    // A function used in the assignment operator definition which swaps the member data
    // between two FileLineReader objects.
    void swap(FileLineReader &first, FileLineReader &second);

public:
    // One param constructor which takes in the name of the file to be read.
    explicit FileLineReader(const string &fileName);

    // A copy constructor.
    FileLineReader(const FileLineReader &flr);

    // Defining the assignment operator.
    FileLineReader &operator=(FileLineReader flr);

    // Destructor.
    ~FileLineReader();

    // Getter for the file path.
    [[nodiscard]] string *getFilePath() const;

    // Getter for the file.
    [[nodiscard]] ifstream *getFile() const;

    // Method which returns line by line the lines from a file.
    string *readLineFromFile();
};

// This class handles the storage and retrieval of all the user commands that relate to the setup of the game.
class CommandProcessor {
private:
    // A list which stores the given commands by the user throughout the game which relate to the setup of the game.
    deque<Command *> *commandList;

    // Defining the output operator for the CommandProcessor object.
    friend std::ostream &operator<<(std::ostream &stream, const CommandProcessor &cmd);

    // A function used in the assignment operator definition which swaps the member data
    // between two CommandProcessor objects.
    void swap(CommandProcessor &first, CommandProcessor &second);

    // Method which prompts the user to enter a command in the console and saves it.
    virtual string *readCommand();

    // Method which creates a Command object which corresponds to the given command that the user entered or that was
    // read from a file.
    Command *saveCommand(const string &command);

public:
    // Default constructor.
    explicit CommandProcessor();

    // A copy constructor.
    CommandProcessor(const CommandProcessor &cmdProcessor);

    // Defining the assignment operator.
    CommandProcessor &operator=(CommandProcessor cmdProcessor);

    // Destructor.
    virtual ~CommandProcessor();

    // Method which reads a command, saves it in a Command object, and then returns that object by calling the readCommand
    // and saveCommand methods.
    [[nodiscard]] Command *getCommand(const GameEngine &ge);

    // Method which validates the given Command object, given the current state of the game. If the method is valid, then
    // it parses the command in order to extract all the keywords from the command and stores it in the command. The method
    // returns a tuple made of:
    // <0> A boolean indicating if the command is valid or not
    // <1> A string corresponding to the command keyword without arguments
    // <2> A string saying that the command is valid or why the command is invalid
    tuple<bool, string, string> validate(const GameEngine &ge, const Command &cmd);
};

// An object following the adapter pattern where we adapt the CommandProcessor in order for it to be able to read user
// commands about the game setup from a file instead of the console.
class FileCommandProcessorAdapter : public CommandProcessor {
private:
    // A FileLineReader which will be used to read the file line by line.
    FileLineReader *flr;

    // Defining the output operator for the FileCommandProcessorAdapter object.
    friend std::ostream &operator<<(std::ostream &stream, const FileCommandProcessorAdapter &cmd);

    // A function used in the assignment operator definition which swaps the member data
    // between two FileCommandProcessorAdapter objects.
    void swap(FileCommandProcessorAdapter &first, FileCommandProcessorAdapter &second);

    // Method which allows the game to read the user commands about the game setup from a file.
    string *readCommand() override;

public:
    // One param constructor which takes in a commandline string argument that corresponds to the name of the file to be read.
    explicit FileCommandProcessorAdapter(const string &fileName);

    // A copy constructor.
    FileCommandProcessorAdapter(const FileCommandProcessorAdapter &fcpa);

    // Defining the assignment operator.
    FileCommandProcessorAdapter &operator=(FileCommandProcessorAdapter fcpa);

    // Destructor.
    ~FileCommandProcessorAdapter() override;

    // Getter for the FileLineReader object.
    [[nodiscard]] FileLineReader *getFileLineReader() const;
};

// Method which deletes the whitespaces on the left of the given string.
static void ltrim(std::string &s);

// Method which deletes the whitespaces on the right of the given string.
static void rtrim(std::string &s);

// Method which deletes the whitespaces on the left and right sides of the given string (by calling ltrim and rtrim).
static void trim(std::string &s);

#endif //COMP_345_PROJECT_TEAM_N12_COMMAND_PROCESSING_H
