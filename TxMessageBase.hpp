#pragma once
#include <cstddef>
#include <vector>
#include <boost/asio/ip/udp.hpp>

class TxMessageBase
{
public:
	enum class MessageType
	{
		SessionOpened = 0x1501,
		MessageForwardingToDestUser = 0x2501,
		MessageForwardingToAllUsers = 0x2502,
		OnlineList = 0x8501,
		ResponseToIAmOnline = 0x8502,
		SessionStartRequestRejected = 0x9501,
		DestinationIsNotAlive = 0x9502,
		ClientMessageWasCorrupted = 0x9503,
		Null
	};

	MessageType type{};

	//dtor
	virtual ~TxMessageBase() {}

	void SendMsg(boost::asio::ip::udp::socket& socket)
	{
		PrepareMessageToSend();
		socket.send_to(boost::asio::buffer(entireMessage), *m_endpoint); //synchronous
	}

	//TODO: AddToPayload may be function template

	//overload for message content
	//TODO: Refactor
	void AddToPayload(std::string s)
	{
		//const auto required_msg_pack = (s.size() / 32) + 1;
		const auto required_msg_pack = (s.size() % 32) == 0 ? s.size() / 32 : s.size() / 32 + 1;
		const unsigned required_byte = required_msg_pack * 32;

		for (auto val : s)
		{
			payload.push_back(val);
		}

		for (int i{}; i < required_byte - s.size(); ++i)
		{
			payload.push_back(0x00);
		}

	}

	void AddToPayload(unsigned int ui)
	{
		payload.push_back(((ui >> 24) & 0xFF));
		payload.push_back(((ui >> 16) & 0xFF));
		payload.push_back(((ui >> 8) & 0xFF));
		payload.push_back(((ui >> 0) & 0xFF));
	}
	void AddToPayload(int i)
	{
		payload.push_back(((i >> 24) & 0xFF));
		payload.push_back(((i >> 16) & 0xFF));
		payload.push_back(((i >> 8) & 0xFF));
		payload.push_back(((i >> 0) & 0xFF));
	}
	void AddToPayload(std::vector<std::byte> content) {}

	//boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), server_port)
	//TODO: Must be set
	boost::asio::ip::udp::endpoint* m_endpoint;


private:
	std::vector<unsigned char> entireMessage{};
	std::vector<unsigned char> payload{};

	void AddHeader()
	{
		entireMessage.push_back(0xFF);
		entireMessage.push_back(0xFF);
		entireMessage.push_back(0xFF);
		entireMessage.push_back(0xFF);
	}

	//TODO:refactor code repetation
	void AddPayloadSize()
	{
		auto size = payload.size();

		entireMessage.push_back(((size >> 24) & 0xFF));
		entireMessage.push_back(((size >> 16) & 0xFF));
		entireMessage.push_back(((size >> 8) & 0xFF));
		entireMessage.push_back(((size >> 0) & 0xFF));
	}

	void AddType()
	{
		auto type_val = (unsigned int)type;

		entireMessage.push_back(((type_val >> 24) & 0xFF));
		entireMessage.push_back(((type_val >> 16) & 0xFF));
		entireMessage.push_back(((type_val >> 8) & 0xFF));
		entireMessage.push_back(((type_val >> 0) & 0xFF));
	}

	//TODO: refactor
	void AddSenderUserName()
	{
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back('s');
		entireMessage.push_back('e');
		entireMessage.push_back('r');
		entireMessage.push_back('v');
		entireMessage.push_back('e');
		entireMessage.push_back('r');
	}

	void AddPayloadToMsg()
	{
		for (auto elem : payload)
		{
			entireMessage.push_back(elem);
		}
	}

	//TODO: Not implemented yet
	void AddChecksum()
	{
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
		entireMessage.push_back(0x00);
	}

	void PrepareMessageToSend()
	{
		AddHeader();
		AddPayloadSize();
		AddType();
		AddSenderUserName();
		AddPayloadToMsg();
		AddChecksum();
	}
};
