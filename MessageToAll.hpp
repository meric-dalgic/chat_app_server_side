#pragma once
#include "Executer.hpp"
#include "RxMessageBase.hpp"
#include "MessageForwardingToAllUsers.hpp"

class MessageToAll : public RxMessageBase
{
public:

	//TODO fill it
	void Parse() override
	{
		for (int i{}; i < payloadSize; ++i)
		{
			if (payload[i] != '\0')
			{
				m_msg += payload[i];
			}
		}
	}

	void DoJob(Utilities::Wrappers::ConcurrentQueue<TxMessageBase*>* queue) override
	{
		if (SessionIsAlreadyActive())
		{
			for(auto session: Executer::ActiveSessionList)
			{
				auto ptr = new MessageForwardingToAllUsers(userName,m_msg);
				ptr->m_endpoint = session->m_endpoint;
				queue->Enqueue(ptr);
			}
		}

		else
		{
			//Do nothing. Session is not active.
		}
	}

private:
	std::string m_msg{};

};
