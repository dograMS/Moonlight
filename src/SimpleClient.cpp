
#include "moonlight.hpp"


int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		std::cout << "CommandLine argument missing\n";
		return -1;
	}
	
	MoonLight ml(argv[1]);
	bool run = true;
	while(run)
	{
		ml.LogClientScheme();
	}
	
	return 0;
}