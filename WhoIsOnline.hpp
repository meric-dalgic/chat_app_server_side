#pragma once
#include "OnlineList.hpp"
#include "RxMessageBase.hpp"

class WhoIsOnline : public RxMessageBase
{
public:

	//TODO fill it
	void Parse() override
	{
		//payload should be 0
		auto payload =Parse4ConsecutiveByte(0);
	}

	void DoJob(Utilities::Wrappers::ConcurrentQueue<TxMessageBase*>* queue) override
	{
		if (SessionIsAlreadyActive())
		{
			std::string online_list;

			for(auto session: Executer::ActiveSessionList)
			{
				online_list += session->m_username + " ";
			}

			auto ptr = new OnlineList(online_list);
			ptr->m_endpoint = m_endpoint;
			queue->Enqueue(ptr);
		}

		else
		{
			//Do nothing. Session is not active.
		}
	}

private:

};
