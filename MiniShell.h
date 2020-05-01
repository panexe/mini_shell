//
// Created by lars on 28.04.20.
//

#ifndef MINI_SHELL_MINISHELL_H
#define MINI_SHELL_MINISHELL_H


#include <vector>
#include <list>
#include <string>
#include "ShellKeyboard.h"
#include <termios.h>

class MiniShell {
public:
    MiniShell();
    ~MiniShell();
    void run();

private:
    struct termios old_settings;
    bool running;

    std::vector<std::list<char>> commandHistory;
    int historyIndex;

    std::list<char>* currentCommand();

    std::string readLine();
    char **parseLine(std::string line, const char& deliminator=' ');

};


#endif //MINI_SHELL_MINISHELL_H
