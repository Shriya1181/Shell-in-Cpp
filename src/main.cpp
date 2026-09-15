#include <iostream>
#include <string>

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  std::string command;
  // TODO: Uncomment the code below to pass the first stage
  std::cout << "$ ";
  //cin reads input till the first whitespace character while getline reads the entire line including whitespace characters
  std::getline(std::cin, command);
  std:: cerr << command << ": command not found"; 
}
