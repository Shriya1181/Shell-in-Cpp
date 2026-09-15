#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  while(true) {
    std::string command;
    // TODO: Uncomment the code below to pass the first stage
    std::cout << "$ ";
    //cin reads input till the first whitespace character
    //getline reads the entire line including whitespace characters
    std::getline(std::cin, command);
    if(command == "exit") {
      //break exits the while loop and terminates the program
      return 0;
    }
    std:: cerr << command << ": command not found" << std::endl; 
  }

}

//REPL code loop - Read Evaluate Print Loop is the cycle the shell follows till it is terminated.
//After it evaluates a command, it prints error (as of now) and waits for next command with $

