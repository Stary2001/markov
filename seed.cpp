#include <ctime>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <regex>
#include "markov.h"

int main(int argc, char** argv)
{
	if(argc < 3)
	{
		std::cout << "Usage: " << argv[0] << " [dictionary] [seed file]" << std::endl;
		return 0;
	}

	int r;
	int fd = open("/dev/urandom", O_RDONLY);
	read(fd, (char*) &r, sizeof(r));
	std::srand(r);
	close(fd);

	markov_dict dict = load(argv[1]);

	std::ifstream is(argv[2]);
	if(!is) return 1;
	is.seekg(0, is.end);
	int len = is.tellg();
	is.seekg(0, is.beg);

	char *str = new char[len];
	is.read(str, len);
	std::string s(str);
	std::regex reg("\r?\n");
	std::string fixed = std::regex_replace(s, reg, std::string(" "));

	seed(dict, fixed);
	delete str;
	save(dict, argv[1]);

	return 0;
}
