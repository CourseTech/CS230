#include <tsh.h>
#include <fstream>
#include <vector>
#include <cstring>

using namespace std;

void tokenize(char* cmd, char** cmdTokens) {
  int i = -1;
  string build = "";
  char* token = strtok (cmd," \n");
  while (token != NULL)
  {
    cmdTokens[++i] = token;
    token = strtok (NULL, " ,.-");
  }
  cmdTokens[++i] = NULL;
}

void simple_shell::parse_command(char* cmd, char** cmdTokens) {
    tokenize(cmd, cmdTokens);
}

void simple_shell::exec_command(char** argv) {
    int pid = fork();
    if (pid == 0) {
        execvp(argv[0], argv);
    } else {
        waitpid(pid, NULL, 0);
    }
}

bool simple_shell::isQuit(char* cmd) {
    return strcmp(cmd, "quit") == 0;
}
