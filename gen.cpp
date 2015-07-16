#include <ctime>
#include <fcntl.h> 
#include <unistd.h>
#include <fstream> 
#include <iostream> 
#include "markov.h"

int main(int argc, char** argv)
{
	if(argc < 2)
	{
                std::cout << "Usage: " << argv[0] << " [dictionary]" << std::endl;
		return 0;
	}

	int r;
        int fd = open("/dev/urandom", O_RDONLY);
	read(fd, (char*) &r, sizeof(r));
	std::srand(r);
	close(fd);

	markov_dict dict = load(argv[1]);

	std::cout << generate(dict, 10) << std::endl ;
	return 0;
}
