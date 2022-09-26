#include <map>
#include <queue>
#include <set>
#include <string>
#include <fstream>
#include <vector>
#include <inverter.h>

using namespace std;

void tokenize(string &str, vector<string> &words)
{
  string build = "";
  for (string::iterator itr = str.begin(); itr != str.end(); ++itr)
  {
    char c = *itr;
    if (isalpha(c))
    {
      build += c;
    }
    else if (build.length() > 0)
    {
      words.push_back(build);
      build = "";
    }
  }
  if (build.length() > 0)
  {
    words.push_back(build);
  }
}

void process_file(int file_id, ifstream &file, map<string, set<int>> &index)
{
  string line;
  map<string, set<int>>::iterator mitr;
  while (getline(file, line))
  {
    vector<string> w;
    tokenize(line, w);
    for (vector<string>::iterator vitr = w.begin(); vitr != w.end(); ++vitr)
    {
      mitr = index.find(*vitr);
      if (mitr != index.end())
      {
        mitr->second.insert(file_id);
      }
      else
      {
        set<int> s;
        s.insert(file_id);
        index.insert(pair<string, set<int>>(*vitr, s));
      }
    }
  }
}

string print_results(map<string, set<int>> &word_map)
{
  map<string, set<int>>::iterator mitr;
  string result = "";
  for (map<string, set<int> >::iterator mitr = word_map.begin(); mitr != word_map.end(); ++mitr)
  {
    cout << mitr->first << ":";
    result += mitr->first;
    result += ":";
    for (set<int>::iterator sitr = mitr->second.begin(); sitr != mitr->second.end(); ++sitr)
    {
      cout << " " << *sitr;
      result += " ";
      result += to_string(*sitr);
    }
    cout << endl;
    result += "\n";
  }
  return result;
}


string build_inverted_index(string main_file_name)
{
  ifstream main_file(main_file_name);
  map<string, set<int>> inverted_index;
  int file_id = -1;
  if (main_file.is_open())
  {
    string file_name;
    while (getline(main_file, file_name))
    {
      ifstream file(file_name);
      if (file.is_open())
      {
        process_file(++file_id, file, inverted_index);
      }
    }
  }
  else
  {
    return "Enter a valid input file name";
  }
  main_file.close();
  return print_results(inverted_index);
}
