#pragma once
#include "RxMessageBase.hpp"
#include <thread>
#include <boost/asio.hpp>
#include "ConcurrentQueue.hpp"
#include <iostream>
#include "MessagePack.hpp"
#include "Session.hpp"
#include "TxMessageBase.hpp"

class Executer
{
public:

	Executer(unsigned int server_port)
	{
		socket.open(boost::asio::ip::udp::v4());
		socket.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), server_port));

		ProducerEnabled = true;
		ConsumerEnabled = true;
		TransmitterEnabled = true;
	}

	void TransmitData();

	void Producer();

	void Consumer();

	inline static std::vector<Session*> ActiveSessionList{};

	Utilities::Wrappers::ConcurrentQueue<MessagePack*>   RawDataQueue{ 10240 };
	//Utilities::Wrappers::ConcurrentQueue<RxMessageBase*> ReceivedMessageLogs{ 10240 };
	Utilities::Wrappers::ConcurrentQueue<TxMessageBase*> TransmitMessageQueue{ 10240 };

private:


	boost::asio::io_service io_service;
	boost::asio::ip::udp::socket socket{ io_service };
	bool TransmitterEnabled{};
	bool ProducerEnabled{};
	bool ConsumerEnabled{};

	std::thread TransmitterThread{ [this]() {TransmitData(); } };
	std::thread ProducerThread{ [this]() {Producer(); } };
	std::thread ConsumerThread{ [this]() {Consumer(); } };


	unsigned int Parse4ConsecutiveByte(Utilities::Wrappers::ConcurrentQueue<unsigned char>* queue)
	{

		int b1 = queue->Dequeue();
		int b2 = queue->Dequeue();
		int b3 = queue->Dequeue();
		int b4 = queue->Dequeue();

		unsigned ret = ((b1 << 24) | (b2 << 16) | (b3 << 8) | (b4 << 0));

		return ret;
	}

	std::string ParseNConsecutiveByte(Utilities::Wrappers::ConcurrentQueue<unsigned char>* queue, std::size_t datalength)
	{

		std::string s{};

		for (int i{}; i < datalength; ++i)
		{
			auto c = queue->Dequeue();
			if (c != '\0')
			{
				s += c;
			}
		}

		return s;
	}

	bool ChecksumValidator(const std::vector<unsigned char>& vec, unsigned int checksum)const
	{
		//unsigned int calculated{};

		//for(auto val : vec)
		//{
		//	calculated += (unsigned char)val;
		//}

		//return calculated == checksum;

		return true;
	}


};
