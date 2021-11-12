#include <iostream>
#include <game_engine/game_engine.h>
#include <algorithm>
#include "command_processing.h"

#define CMD_DIR "../commands/"

// One param constructor which creates a Command object with a string corresponding to the entire command entered by
// the user or read from the file.
Command::Command(const string &command) {
    this->commandName = new string(command);
    this->commandEffect = nullptr;
    this->commandArgs = new vector<string *>();
}

// Copy constructor.
Command::Command(const Command &cmd) {
    this->commandName = new string(*cmd.commandName);
    this->commandEffect = new string(*cmd.commandEffect);
    this->commandArgs = new vector<string *>(*cmd.commandArgs);
}

// Swaps the member data between two Command objects.
void Command::swap(Command &first, Command &second) {
    std::swap(first.commandName, second.commandName);
    std::swap(first.commandEffect, second.commandEffect);
    std::swap(first.commandArgs, second.commandArgs);
}

// Destructor.
Command::~Command() {
    delete commandName;
    delete commandEffect;
    delete commandArgs;
}

// Assignment operator.
Command &Command::operator=(Command cmd) {
    swap(*this, cmd);
    return *this;
}

// Defining the output operator for the Command class.
std::ostream &operator<<(ostream &stream, const Command &cmd) {
    string effect = "Not executed yet";

    if (cmd.commandEffect != nullptr) {
        effect = *cmd.getCommandEffect();
    }

    stream << "----Command information----\nCommand Name: " << *cmd.getCommandName() << "\nCommand Effect: " << effect
           << "\nCommand Arguments:";

    if (cmd.getCommandArgs()->empty()) {
        stream << " Not parsed yet";
    } else {
        for (int i = 0; i < cmd.getCommandArgs()->size(); i++) {
            stream << "\n" << " " << i + 1 << ": " << *cmd.getCommandArgs()->at(i);
        }
    }

    return stream;
}

// Getter for the command name.
string *Command::getCommandName() const {
    return commandName;
}

// Getter for the command effect.
string *Command::getCommandEffect() const {
    return commandEffect;
}

// Getter for the command arguments (keywords from the command).
vector<string *> *Command::getCommandArgs() const {
    return commandArgs;
}

// Method which saves the effect of the command given a certain effect and given whether the command is valid or not.
void Command::saveEffect(const string &newEffect, bool isCommandValid) {
    std::string cmd = *this->getCommandName();
    auto argStart = cmd.find(' ');

    if (isCommandValid && argStart != string::npos) {
        this->commandEffect = new string(newEffect + " " + cmd.substr(argStart + 1, cmd.size() + 1));
    } else {
        this->commandEffect = new string(newEffect);
    }
}

// One param constructor which takes in the name of the file to be read.
FileLineReader::FileLineReader(const string &fileName) {
    cout << "Received file name: " << fileName << endl;
    this->filePath = new string(CMD_DIR + fileName);
    this->file = new ifstream(*this->filePath, std::ifstream::in);
}

// Copy constructor.
FileLineReader::FileLineReader(const FileLineReader &flr) {
    this->filePath = new string(*flr.filePath);
    this->file = new ifstream(*flr.filePath);
}

// Destructor.
FileLineReader::~FileLineReader() {
    delete filePath;
    delete file;
}

// Swaps the member data between two FileLineReader objects.
void FileLineReader::swap(FileLineReader &first, FileLineReader &second) {
    std::swap(first.filePath, second.filePath);
    std::swap(first.file, second.file);
}

// Assignment operator for FileLineReader.
FileLineReader &FileLineReader::operator=(FileLineReader flr) {
    swap(*this, flr);
    return *this;
}

// Defining the output operator for the FileLineReader class.
std::ostream &operator<<(ostream &stream, const FileLineReader &flr) {
    return stream << "FileLineReader info:" << "\nPath to file: " << *flr.filePath << "\nIs file open: "
                  << flr.file->is_open();
}

// Getter for the file path.
string *FileLineReader::getFilePath() const {
    return this->filePath;
}

// Getter for the file.
ifstream *FileLineReader::getFile() const {
    return this->file;
}

// Method which returns line by line the lines from a file.
string *FileLineReader::readLineFromFile() {
    string input;

    cout << "\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;

    if (file->eof()) {
        return nullptr;
    }

    getline(*file, input);
    trim(input);

    cout << "\nReading the new following line from file: " << input << endl;

    return new string(input);
}

// Default constructor.
CommandProcessor::CommandProcessor() {
    this->commandList = new deque<Command *>();
}

// Copy constructor.
CommandProcessor::CommandProcessor(const CommandProcessor &cmdProcessor) {
    this->commandList = new deque<Command *>(*cmdProcessor.commandList);
}

// Destructor.
CommandProcessor::~CommandProcessor() {
    delete commandList;
}

// Defining the output operator for the CommandProcessor class.
std::ostream &operator<<(ostream &stream, const CommandProcessor &cmdProcessor) {
    int i = 1;

    stream << "Printing the list of commands inside the CommandProcessor:\n";

    for (auto it = cmdProcessor.commandList->cbegin(); it != cmdProcessor.commandList->cend(); it++) {
        stream << "\n" << i << ":" << **it << endl;
        i++;
    }

    return stream;
}

// Swaps the member data between two CommandProcessor objects.
void CommandProcessor::swap(CommandProcessor &first, CommandProcessor &second) {
    std::swap(first.commandList, second.commandList);
}

// Assignment operator implementation for the CommandProcessor class.
CommandProcessor &CommandProcessor::operator=(CommandProcessor cmdProcessor) {
    swap(*this, cmdProcessor);
    return *this;
}

// Method which reads a command, saves it in a Command object, and then returns that object by calling the readCommand
// and saveCommand methods.
Command *CommandProcessor::getCommand(const GameEngine &ge) {
    string *cmd = readCommand();

    if (cmd == nullptr) {
        cout << "\nNo more commands left to read." << endl;
        return nullptr;
    }

    Command *command = saveCommand(*cmd);

    delete cmd;
    return command;
}

// Method which prompts the user to enter a command in the console and saves it.
string *CommandProcessor::readCommand() {
    string cmd;

    cout << "\nPlease enter the word(s) corresponding to the action you wish to take:" << endl;

    getline(cin, cmd);
    trim(cmd);

    cout << "\nYou entered option: " << cmd << endl;

    return new string(cmd);
}

// Method which creates a Command object which corresponds to the given command that the user entered or that was
// read from a file.
Command *CommandProcessor::saveCommand(const string &command) {
    auto *cmd = new Command(command);

    commandList->push_back(cmd);

    cout << "\nAdding new command.\n\n" << *this << endl;

    return cmd;
}

// Method which validates the given Command object, given the current state of the game. If the method is valid, then
// it parses the command in order to extract all the keywords from the command and stores it in the command. The method
// returns a tuple made of:
// <0> A boolean indicating if the command is valid or not
// <1> A string corresponding to the command keyword without arguments
// <2> A string saying that the command is valid or why the command is invalid
tuple<bool, string, string> CommandProcessor::validate(const GameEngine &ge, const Command &cmd) {
    vector<string> *actions = ge.getStateMap()->at(*ge.getCurrentState());
    string command;
    string delim = " ";
    string result;
    int argCounter = 0;
    auto start = 0U;
    auto end = cmd.getCommandName()->find(delim);

    // If there is no space in the command, then the command has no arguments and can be directly used.
    if (end == string::npos) {
        command = *cmd.getCommandName();
        cmd.getCommandArgs()->emplace_back(new string(command));
    } else { // Else, there are arguments in the command.

        // Take the first word as the command keyword and add that keyword to the list of arguments.
        command = cmd.getCommandName()->substr(start, end);
        cmd.getCommandArgs()->emplace_back(new string(command));

        // Count the number of arguments in the command and store the arguments in the arguments list of the command.
        while (end != std::string::npos) {
            argCounter++;
            start = end + delim.length();
            end = cmd.getCommandName()->find(delim, start);
            cmd.getCommandArgs()->emplace_back(new string(cmd.getCommandName()->substr(start, end - start)));
        }
    }

    // Iterate over the allowed actions in the current state and check if the first keyword of the command matches to one
    // of those allowed actions.
    for (const string &action: *actions) {
        if (action == command) {
            if (argCounter == get<1>(ge.getDescriptionMap()->at(command))) {
                result = "Command is valid.";
                cout << "\n" << result << endl;
                return make_tuple(true, command, result);
            } else { // If the number of counted arguments is not equal to the number of required arguments for the command, then it is an invalid command.
                result =
                        "The command \"" + *cmd.getCommandName() + "\" has the wrong number of parameters. Expected: " +
                        to_string(get<1>(ge.getDescriptionMap()->at(command))) + ", Obtained: " +
                        to_string(argCounter);
                cout << "\n" << result << endl;
                return make_tuple(false, command, result);
            }
        }
    }

    result = "The command " + command + " does not exist in the current state \"" + *ge.getCurrentState() + "\".";
    cout << "\n" << result << endl;
    return make_tuple(false, command, result);
}

// One param constructor which takes in a commandline string argument that corresponds to the name of the file to be read.
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const string &fileName)
        : CommandProcessor() {
    flr = new FileLineReader(fileName);
}

// Copy constructor.
FileCommandProcessorAdapter::FileCommandProcessorAdapter(const FileCommandProcessorAdapter &fcpa) {
    this->flr = new FileLineReader(*fcpa.flr);
}

// Destructor.
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete flr;
}

// Defining the output operator for the FileCommandProcessorAdapter class.
std::ostream &operator<<(ostream &stream, const FileCommandProcessorAdapter &fcpa) {
    return stream << "FileCommandProcessorAdapter information:\n" << *fcpa.getFileLineReader();
}

// Swaps the member data between two FileCommandProcessorAdapter objects.
void FileCommandProcessorAdapter::swap(FileCommandProcessorAdapter &first, FileCommandProcessorAdapter &second) {
    std::swap(first.flr, second.flr);
}

// Assignment operator for the FileCommandProcessorAdapter class.
FileCommandProcessorAdapter &FileCommandProcessorAdapter::operator=(FileCommandProcessorAdapter fcpa) {
    swap(*this, fcpa);
    return *this;
}

// Getter for the FileLineReader object.
FileLineReader *FileCommandProcessorAdapter::getFileLineReader() const {
    return this->flr;
}

// Method which allows the game to read the user commands about the game setup from a file.
string *FileCommandProcessorAdapter::readCommand() {
    return flr->readLineFromFile();
}

// trim from start (in place)
static void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}
