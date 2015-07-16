#include <unordered_map>
#include <string>

typedef std::unordered_map<std::string, unsigned int> markov_state;
typedef std::unordered_map<std::string, markov_state> markov_dict; 
typedef markov_dict::value_type markov_pair;
typedef markov_dict::iterator markov_dict_iterator;

void seed(markov_dict &dict, std::string string);
std::string generate(markov_dict dict, unsigned int len);
markov_dict load(std::string filename);
void save(markov_dict dict, std::string filename);
