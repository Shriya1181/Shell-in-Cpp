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
      //break exits the while loop and terminates the program or can use return 0
      break;
    //substr returns a substring of the string starting from the index specified and of length specified
    } else if(command.substr(0, 5) == "echo ") {
      std::cout << command.substr(5) << std::endl;
    } else {
    std:: cerr << command << ": command not found" << std::endl; 
    }
  }

}

//REPL code loop - Read Evaluate Print Loop is the cycle the shell follows till it is terminated.
//After it evaluates a command, it prints error (as of now) and waits for next command with $

