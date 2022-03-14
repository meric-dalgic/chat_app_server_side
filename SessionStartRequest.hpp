#pragma once
#include "Executer.hpp"
#include "RxMessageBase.hpp"
#include "SessionOpened.hpp"
#include "SessionStartRequestRejected.hpp"

class SessionStartRequest : public RxMessageBase
{
public:

	//TODO fill it
	void Parse() override
	{
		alive_msg_period = Parse4ConsecutiveByte(0);
	}

	void DoJob(Utilities::Wrappers::ConcurrentQueue<TxMessageBase*>* queue) override
	{
		if (SessionIsAlreadyActive())
		{
			auto ptr = new SessionStartRequestRejected(SessionStartRequestRejected::Reason::SessionIsAlreadyActive);
			ptr->m_endpoint = m_endpoint;
			queue->Enqueue(ptr);
		}

		else
		{
			Executer::ActiveSessionList.push_back((new Session(m_endpoint, userName, alive_msg_period)));

			auto ptr = new SessionOpened(userName);
			ptr->m_endpoint = m_endpoint;
			queue->Enqueue(ptr);
		}
	}

private:
	unsigned int alive_msg_period{};

};
