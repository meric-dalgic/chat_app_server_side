#pragma once
#include <iostream>
#include <boost/asio/ip/udp.hpp>
#include "Executer.hpp"

class Session
{
public:
	enum class State
	{
		Alive,
		Dead
	};

	Session(boost::asio::ip::udp::endpoint* endpoint, std::string username, unsigned int alive_msg_priod) :
	m_endpoint(endpoint), m_username(username), m_alive_msg_period(alive_msg_priod)
	{
		
	}

	void AliveCheck()
	{
		while (thread_enabled)
		{
			unsigned int prev_value{ m_alive_msg_counter };
			std::this_thread::sleep_for(std::chrono::milliseconds(m_alive_msg_period * 2));
			unsigned int next_value{ m_alive_msg_counter };

			//Session was closed
			if (next_value == prev_value)
			{
				std::cout << "##Session for username: " << m_username << " will be closed due to responsiveness.\n";
				thread_enabled = false;
				RemoveRelatedSession(m_username);
			}
		}
	}


	boost::asio::ip::udp::endpoint* m_endpoint;
	std::string m_username{};
	std::string m_lastAliveMsg{};
	unsigned int m_alive_msg_period{};
	unsigned int m_alive_msg_counter{};
	bool thread_enabled{ true };

	//Her obje requeste karþýlýk yaratýlýyor hali hazýrda, bu sebeple yaratýldýðý anda aslýnda alive.
	State currentState{ State::Alive };


	Session* FindRelatedSession(std::string);
	bool RemoveRelatedSession(std::string);
private:
	std::thread AliveChecker{ [this]() {AliveCheck(); } };

};
