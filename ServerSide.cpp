#include <iostream>
#include <string>
#include "Executer.hpp"

//const unsigned int server_port = 16000;

int main()
{
    unsigned int server_port{};

    //server port
    while (true)
    {
        std::string temp{};

        std::cout << "Please enter port of server\n";
        std::getline(std::cin, temp);

        //validation checker function will be added for all checking operations
        if (false)
        {

        }

        else
        {
            server_port = atoi(temp.c_str());
            break;
        }
    }


	Executer exec(server_port);

	std::string command{};


	while (true)
	{
		while (true)
		{
			std::cout << "Please enter a command\n";
			std::getline(std::cin, command);
		}
	}

	return 0;
}

