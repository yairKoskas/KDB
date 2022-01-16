#include <iostream>
#include <unistd.h>
#include <csignal>

#include "Debugger.hpp"
#include "DebuggerManager.hpp"

void signal_handler(const int signal_nr, DebuggerManager* ptr) {
    signal(signal_nr, (void (*)(int))signal_handler);
    // hacky solution I know but whatever
    static DebuggerManager* debuggerManager = nullptr;
    if (debuggerManager == nullptr) {
        debuggerManager = ptr;
    }
    if (signal_nr == SIGCHLD) {
        debuggerManager->setRunning(false);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: kdb [path_to_executable]" << std::endl;
        return -1;
    }
    if (access(argv[1], F_OK)) {
        std::cout << "File " << argv[1] << " doesn't exist" << std::endl;
        return -1;
    }
    if (access(argv[1], X_OK)) {
        std::cout << "File " << argv[1] << " isn't executable, maybe chmod +x?" << std::endl;
        return -1;
    }
    auto* elf = new ELF(argv[1]);
    auto* debugger = new Debugger(new Debuggee(elf));
    auto* manager = new DebuggerManager(debugger);
    signal(SIGCHLD, (void (*)(int))signal_handler);
    // initialize the debugger in the signal_handler function
    signal_handler(-1, manager);
    while (true) {
        while (!manager->isRunning()) {
            if (manager->checkStatus() == Debugger::DebuggeeStatus::EXITED) {
                std::cout << "Thanks for using my debugger, goodbye!" << std::endl;
                std::exit(1);
            }
            // manager's status is STOPPED
            //restore the breakpoint
            std::string command;
            std::cout << "$ ";
            std::getline(std::cin, command);
            manager->execute(command);
            if (command == "c") {
                manager->setRunning(true);
            }
        }
    }
}