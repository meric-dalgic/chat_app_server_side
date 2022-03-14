#pragma once
#include <string>
#include <vector>
#include "Executer.hpp"
#include "TxMessageBase.hpp"
#include "ConcurrentQueue.hpp"
#include "Session.hpp"

class RxMessageBase
{
public:
	enum class MessageType
	{
		SessionStartRequest = 0x1001,
		MessageToUser = 0x2001,
		MessageToAll = 0x2002,
		WhoIsOnline = 0x8001,
		IAmOnline = 0x8002,
		ClientMessageWasCorrupted = 0x9003,
		Null
	};

	std::string dateTime{};
	MessageType type{};
	std::vector<unsigned char> payload{};
	unsigned int payloadSize{};

	//Ctor, date&time will be assigned inside.
	RxMessageBase() = default;

	//dtor
	virtual ~RxMessageBase() = default;

	unsigned int Parse4ConsecutiveByte(unsigned int packageIndex)
	{
		unsigned int value{};

		for (unsigned int i = 0 + (packageIndex * 4); i < 4 + (packageIndex * 4); i++)
		{
			value |= ((unsigned char)payload[i]) << 8 * (3 - i + (packageIndex * 4));
		}

		return value;
	}

	std::string ParseNConsecutiveByte(std::size_t datalength, int startIndex)
	{

		std::string s{};

		for (int i{ startIndex }; i < startIndex + datalength; ++i)
		{
			s += payload[i];
		}

		return s;
	}

	virtual void Parse() = 0;
	virtual void DoJob(Utilities::Wrappers::ConcurrentQueue<TxMessageBase*>*) = 0;

	static RxMessageBase* CreateInstance(MessageType);

	boost::asio::ip::udp::endpoint* m_endpoint;
	std::string userName{};

	bool SessionIsAlreadyActive();

	Session* FindSession(std::string);
private:
};
