#include <iostream>
#include <string>
#include <set>
#include <cstdlib> // Required for std::getenv (from C library)
#include <sstream>
#include <filesystem> // Required for checking files and permissions
#include <vector>
//To execute a command in a child process, we need to include the following headers as exec() replaces the current process image with a new process image
#include <unistd.h>    // For fork() and execv()
//use waitpid() to wait for the child process to finish and retrieve its exit status (before printing $ again)
#include <sys/wait.h>
#include <utility>

namespace fs = std::filesystem; // Alias for easier access to filesystem namespace

#ifdef _WIN32
constexpr char PATH_LIST_SEPARATOR = ';';
#else
constexpr char PATH_LIST_SEPARATOR = ':';
#endif

std::vector<std::string> split_input(const std::string& input) {
    std::stringstream ss(input);
    std::string word;
    std::vector<std::string> arguments;
    // The >> operator automatically splits by spaces and handles multiple spaces cleanly
    while (ss >> word) {
        arguments.push_back(word);
    }
    return arguments;
}

std::pair<bool, fs::path> is_in_dir(const std::vector<std::string>& path_dirs, const std::string& cmd) {
    for (const std::string& dir : path_dirs) {
          //forward-slash (/) operator is overloaded. Instead of doing math, it joins paths together intelligently in filesystem library
          fs::path full_path = fs::path(dir) / cmd;
          //check if file exists at the path, is a file and not a folder/directory, and has execute permissions for the owner.
          //To check permissions, we use bitwise AND operator (&) to check if the owner_exec permission is set. If it is not equal to none, it means the file has execute permissions for the owner.
          if (fs::exists(full_path) && fs::is_regular_file(full_path) && (fs::status(full_path).permissions() & fs::perms::owner_exec) != fs::perms::none) {
            return std::make_pair(true, full_path);
          }
        }
    return std::make_pair(false, fs::path());
}

void execute(const std::vector<std::string>& args, const std::pair<bool, fs::path>& result) {
    std::vector<char*> c_args;
    for (const auto& arg : args) {
    //cast to char* as execv expects an array of char* (C-style strings) and not std::string
      c_args.push_back(const_cast<char*>(arg.c_str()));
    }
    c_args.push_back(nullptr); // Must end with null pointer to show no more arguments
    //create a new proccess to run the command. Fork() creates a copy of the current process with a new pid.
    pid_t pid = fork();
    //child process given a pid of 0(not true pid), parent process given a pid greater than 0, and if fork fails it returns -1
    //0 lets the child process know it is a child process and should run the command, while the parent process waits for the child to finish execution before continuing.
    if (pid == 0) {
      // Child process runs the binary file. execv takes the path to the binary and an array of arguments (including the command itself as the first argument).
      execv(result.second.c_str(), c_args.data());
      exit(1); // Exit if execv fails
    } else if (pid > 0) {
      // Parent process waits for child to finish execution
      int status;
      waitpid(pid, &status, 0);
    }
}

int main() {
  // Flush after every std::cout / std:cerr (Turn of buffering)
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  std::set<std::string> builtins = {"echo", "type", "exit"};
  //getenv returns a pointer to the value in the environment, or NULL if there is no match. 
  //For PATH this returns a pointer to the value of the PATH environment variable, a list of directories separated by PATH_LIST_SEPARATOR. 
  char* path_env = std::getenv("PATH");
  std::vector<std::string> path_dirs;
  //convert pointer to string and then use stringstream to split the string into list of directories based on the PATH_LIST_SEPARATOR(:) in path_dirs
  if(path_env) {
  //otherwise if there is no input will give segmentation fault as path_env will be NULL and dereferencing it will cause a crash
    std::string path_str(path_env); //converts to C++ style string
    std::stringstream ss(path_str); //converts to stringstream (easy to split)
    std::string directory;
    while (std::getline(ss, directory, PATH_LIST_SEPARATOR)) {
        path_dirs.push_back(directory);
    }
  }
  while(true) {
    std::string input;
    // TODO: Uncomment the code below to pass the first stage
    std::cout << "$ ";
    //cin reads input till the first whitespace character
    //getline reads the entire line including whitespace characters
    std::getline(std::cin, input);
    std::vector<std::string> args = split_input(input);
    if (args.empty()) {
        continue;
    }
    std::string command = args[0];
    if(command == "exit") {
      //break exits the while loop and terminates the program or can use return 0
      break;
    //substr returns a substring of the string starting from the index specified and of length specified
    } else if(command == "echo" && args.size() > 1) {
      for (size_t i = 1; i < args.size(); ++i) {
        //ternary operator checks if the current argument is the last one to decide to add space or not.
        std::cout << args[i] << (i + 1 < args.size() ? " " : "");
      }
      std::cout << std::endl;
    } else if(command == "type" && args.size() == 2) {
      std::string cmd = args[1];
      if (builtins.count(cmd)) {
        std::cout << cmd << " is a shell builtin" <<std::endl;
      } else {
        auto result = is_in_dir(path_dirs, cmd);
        if (result.first) {
          std::cout << cmd << " is " << result.second.string() << std::endl;
        } else {
          std::cout << cmd << ": not found" << std::endl;
        }
      }
    } else {
      auto result = is_in_dir(path_dirs, command);
      if (result.first) {
        execute(args, result);
      } else {
        std::cout << command << ": command not found" << std::endl;
      }
    }
  }
  return 0;
}