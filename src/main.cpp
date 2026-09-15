#include <iostream>
#include <string>
#include <set>
#include <cstdlib>

#ifdef _WIN32
constexpr char PATH_LIST_SEPARATOR = ';';
#else
constexpr char PATH_LIST_SEPARATOR = ':';
#endif

int main() {
  // Flush after every std::cout / std:cerr (Turn of buffering)
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  std::set<std::string> builtins = {"echo", "type", "exit"};
  //getenv returns a pointer to the value in the environment, or NULL if there is no match. 
  //For PATH this returns a pointer to the value of the PATH environment variable, a list of directories separated by PATH_LIST_SEPARATOR. 
  char* path_env = std::getenv("PATH");
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
    } else if(command.substr(0, 5) == "type ") {
      std::string cmd = command.substr(5);
      if (builtins.count(cmd)) {
        std::cout << cmd << " is a shell builtin" <<std::endl;
      } else {
        if (path_env) {
          
        }
        std::cerr << cmd << ": not found" << std::endl;
      }
    } else {
    std::cerr << command << ": command not found" << std::endl; 
    }
  }

}

//REPL code loop - Read Evaluate Print Loop is the cycle the shell follows till it is terminated.
//After it evaluates a command, it prints error (as of now) and waits for next command with $

