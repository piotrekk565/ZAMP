#ifndef COMMANDS_HH
#define COMMANDS_HH
#include <vector>
#include <optional>
#include <sstream>

struct Command {
  std::string name{};
  std::string command{};
  static std::optional<Command> parse(char* line);
};


struct Commands {
  std::vector<Command> commands{};
};

std::optional<Command> Command::parse(char* cLine) {
  std::string sLine = cLine;
  auto seperator_position = sLine.find(' ', 5);

  if(seperator_position == -1) {
    return std::nullopt;
  } else {
    std::string name = sLine.substr(0, seperator_position);
    std::string command = sLine.substr(seperator_position, sLine.length());
    return std::optional<Command>{Command{name: name, command: command}};
  }
}

#endif