#pragma once
#include <boost/asio/ip/address.hpp>
#include <boost/asio/ip/udp.hpp>

#include "ConcurrentQueue.hpp"

class MessagePack
{
public:
	MessagePack(boost::asio::ip::udp::endpoint* endpoint) : m_endpoint(endpoint)
	{
		
	}

	~MessagePack()
	{
		delete m_endpoint;
	}

	//Will be filled after object creation, from producer
	Utilities::Wrappers::ConcurrentQueue<unsigned char> Data{ 1024 };
	boost::asio::ip::udp::endpoint* m_endpoint{};


private:
};
