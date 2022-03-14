#pragma once
#include "DestinationIsNotAlive.hpp"
#include "MessageForwardingToDestUser.hpp"
#include "RxMessageBase.hpp"

class MessageToUser : public RxMessageBase
{
public:

	//TODO fill it
	void Parse() override
	{
		//payload should be 0
		m_destination_user_name = ParseNConsecutiveByte(32,0);

		for (int i{32}; i < payloadSize; ++i)
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
			auto p = FindSession(m_destination_user_name);


			if (p != nullptr)
			{
				auto ptr = new MessageForwardingToDestUser(userName, m_msg);
				ptr->m_endpoint = p->m_endpoint;
				queue->Enqueue(ptr);
			}


			//Send destination is not active message
			else
			{
				auto ptr = new DestinationIsNotAlive(m_destination_user_name);
				ptr->m_endpoint = m_endpoint;
				queue->Enqueue(ptr);
			}



			//std::string online_list;

			//for (auto session : Executer::ActiveSessionList)
			//{
			//	online_list += session.m_username + " ";
			//}

			//auto ptr = new OnlineList(online_list);
			//ptr->m_endpoint = m_endpoint;
			//queue->Enqueue(ptr);
		}

		else
		{
			//Do nothing. Session is not active.
		}
	}

private:
	std::string m_destination_user_name{};
	std::string m_msg{};

};
