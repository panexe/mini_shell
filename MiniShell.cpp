//
// Created by lars on 28.04.20.
//

#include <iostream>
#include <sstream>
#include <cstring>
#include <list>
#include <stdio.h>

#define STDIN_FILENO 0




#include "MiniShell.h"

/* Constructor
 * Enables getting key inputs directly e.g. Arrow-Keys
 */
MiniShell::MiniShell() {
    historyIndex = -1;

    static struct termios  oldt, newt;
    tcgetattr( STDIN_FILENO, &oldt);
    this->old_settings = oldt;
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr( STDIN_FILENO, TCSANOW, &newt);
}

/* Destructor
 * puts console settings back to normal
 */
MiniShell::~MiniShell() {
    tcsetattr( STDIN_FILENO, TCSANOW, &this->old_settings);
}

/* main loop
 * starts the shell
 */
void MiniShell::run() {
    running = true;
    while(running){
        std::string line = this->readLine();
        char** args = this->parseLine(line);
        int i = 0;
        while(args[i] != NULL){
            std::cout << args[i] << '\n';

            i++;
        }
    }


}

/*******************************************************
 *                       User IO                       *
 *                                                     *
 *******************************************************/

/* Gets line from console
 *
 */
std::string MiniShell::readLine() {
    if(commandHistory.size() == 0){
        commandHistory.emplace_back(std::list<char>());
        historyIndex = commandHistory.size()-1;
    }
    else if(historyIndex +1 == commandHistory.size()){
        commandHistory.emplace_back(std::list<char>());
        historyIndex = commandHistory.size()-1;
    }else{
        historyIndex = commandHistory.size()-1;
    }


    std::list<char>* command = currentCommand();
    int cursorPosition = 0;
    int c =0;


    // input loop
    while(true){
        ShellKeyboard::save_cursor();
        ShellKeyboard::setCursor(0,0);
        ShellKeyboard::clear_line();
        std::cout << cursorPosition << "; " << command->size();
        ShellKeyboard::restore_cursor();

        c = getchar();
        // Escape Character
        if(c == 27){
            if((c=getchar())= 91){
                switch((c=getchar())){
                    case 65:
                        if(historyIndex - 1 >= 0){
                            historyIndex--;
                            command = &commandHistory[historyIndex];
                            ShellKeyboard::cursor_left(cursorPosition);
                            ShellKeyboard::clear_line();
                            cursorPosition = 0;
                            for(auto it = command->begin(); it != command->end(); ++it){
                                putchar(*it);
                                cursorPosition++;
                            }
                        }

                        break;
                    case 66:
                        if(historyIndex + 1 <= commandHistory.size()-1){
                            historyIndex++;
                            command = &commandHistory[historyIndex];
                            ShellKeyboard::cursor_left(cursorPosition);
                            ShellKeyboard::clear_line();
                            cursorPosition = 0;
                            for(auto it = command->begin(); it != command->end(); ++it){
                                putchar(*it);
                                cursorPosition++;
                            }
                        }
                        break;
                    case 67:
                        if(cursorPosition+1 <= command->size()){
                            cursorPosition++;
                            ShellKeyboard::cursor_right();
                        }

                        break;
                    case 68:
                        if(cursorPosition -1 >= 0){
                            cursorPosition--;
                            ShellKeyboard::cursor_left();

                        }
                        break;
                    default:
                        std::cout << c << '\n';
                }
            }


        }else if(c == 127){ // Backspace
            if(cursorPosition == 0){
                // do nothing
            }else if(cursorPosition == command->size()){
                command->pop_back();
                ShellKeyboard::backspace();
                cursorPosition--;
            }else{
                ShellKeyboard::save_cursor();

                auto index = command->begin();
                std::advance(index, cursorPosition-1);
                command->erase(index); // delete l

                ShellKeyboard::clear_line();
                ShellKeyboard::cursor_left(cursorPosition--);

                for(auto it = command->begin(); it != command->end(); ++it){
                    putchar(*it);
                }
                ShellKeyboard::restore_cursor();
                ShellKeyboard::cursor_left();
            }
        }
        else if(c == '\n'){ // enter (carriage return)
            putchar('\n');
            std::string ret = "";

            if(historyIndex +1 < commandHistory.size()){
                historyIndex = commandHistory.size()-1;
                commandHistory[historyIndex] = *command;
                command = currentCommand();
            }


            for(auto it = command->begin(); it != command->end(); ++it){
                ret.push_back(*it);
            }


            return ret;
        }else{ // normal letters
            if(cursorPosition !=  command->size()){
                ShellKeyboard::save_cursor();

                auto index = command->begin();
                std::advance(index, cursorPosition);
                command->emplace(index, c);

                ShellKeyboard::clear_line();
                ShellKeyboard::cursor_left(++cursorPosition);
                for(auto it = command->begin(); it != command->end(); ++it){
                    putchar(*it);
                }
                ShellKeyboard::restore_cursor();
                ShellKeyboard::cursor_right();
            }else{
                command->push_back(c);
                cursorPosition++;
                putchar(c);
            }
        }


    }



}

/* splits the given string by the given delimiter
 * returns a char pointer array containing the segments with the last value being NULL
 */
char **MiniShell::parseLine(std::string line,const char& deliminator) {
    std::stringstream text(line);
    std::string seg;
    std::vector<std::string> segments;
    char** ret;

    while(std::getline(text, seg, deliminator)){
        segments.push_back(seg);
    }

    ret = new char*[segments.size() + 1];
    int i=0;
    for(; i<segments.size(); ++i){
        ret[i] = new char[segments[i].length()];
        strcpy(ret[i], segments[i].c_str());
    }
    ret[i] = NULL;
    return ret;
}

std::list<char> *MiniShell::currentCommand() {
    return &commandHistory[historyIndex];
}




