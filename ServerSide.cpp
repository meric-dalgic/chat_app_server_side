#include <iostream>
#include <string>
#include "Executer.hpp"

//TODO: Make this thread safe
std::vector<std::exception> ex_log;

int main()
{
    try
    {
        unsigned int server_port{};

        //server port
        while (true)
        {
            std::string temp{};

            std::cout << "Please enter port of server\n";
            std::getline(std::cin, temp);

            server_port = strtol(temp.c_str(), nullptr, 10);

            //validation checker function will be added for all checking operations
            if (server_port == 0)
            {
                std::cout << "Not a valid port number!\n";
            }

            else
            {
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
    }

    catch (...)
    {
        std::cout << "Unhandled exception occurs\n";
    }

	return 0;
}

