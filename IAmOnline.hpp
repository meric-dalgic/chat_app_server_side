#pragma once
#include "RxMessageBase.hpp"

class IAmOnline : public RxMessageBase
{
public:

	void Parse() override
	{
		//temp = 0 may be checked
		auto temp= Parse4ConsecutiveByte(0);
	}

	void DoJob(Utilities::Wrappers::ConcurrentQueue<TxMessageBase*>* queue) override
	{
		if (SessionIsAlreadyActive())
		{
			auto p = FindSession(userName);
			p->m_alive_msg_counter++;


			//if (p != nullptr)
			//{
			//	auto ptr = new MessageForwardingToDestUser(m_destination_user_name, m_msg);
			//	ptr->m_endpoint = p->m_endpoint;
			//	queue->Enqueue(ptr);
			//}


			////Send destination is not active message
			//else
			//{
			//	auto ptr = new DestinationIsNotAlive(m_destination_user_name);
			//	ptr->m_endpoint = m_endpoint;
			//	queue->Enqueue(ptr);
			//}



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

};
