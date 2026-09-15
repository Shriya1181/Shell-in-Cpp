#include <iostream>
#include <string>
#include <set>
#include <cstdlib> // Required for std::getenv (from C library)
#include <sstream>
#include <filesystem> // Required for checking files and permissions
#include <vector>

namespace fs = std::filesystem; // Alias for easier access to filesystem namespace

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
  //convert pointer to string and then use stringstream to split the string into list of directories based on the PATH_LIST_SEPARATOR(:) in path_dirs
  std::string path_str(path_env); //converts to C++ style string
  std::stringstream ss(path_str); //converts to stringstream (easy to split)
  std::string directory;
  std::vector<std::string> path_dirs;
  while (std::getline(ss, directory, PATH_LIST_SEPARATOR)) {
        path_dirs.push_back(directory);
    }
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
    } else if(command.size() >= 5 && command.substr(0, 5) == "echo ") {
      std::cout << command.substr(5) << std::endl;
    } else if(command.substr(0, 5) == "type ") {
      std::string cmd = command.substr(5);
      if (builtins.count(cmd)) {
        std::cout << cmd << " is a shell builtin" <<std::endl;
      } else {
        bool found = false;
        for (const std::string& dir : path_dirs) {
          fs::path full_path = fs::path(dir) / cmd;
          if (fs::exists(full_path) && fs::is_regular_file(full_path)) {
            std::cout << cmd << " is " << full_path.string() << std::endl;
            found = true;
            break; // Stop searching once we find the first match
          }
        }
        //If could'nt find in any of the directories in PATH, print not found message to stdout
        if (!found) {
          std::cout << cmd << ": not found" << std::endl; // Codecrafters expects stdout for this test stage
        }
      }
    } else {
    std::cerr << command << ": command not found" << std::endl; 
    }
  }

}

//REPL code loop - Read Evaluate Print Loop is the cycle the shell follows till it is terminated.
//After it evaluates a command, it prints error (as of now) and waits for next command with $

