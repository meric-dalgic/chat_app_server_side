#pragma once
#include "TxMessageBase.hpp"


class SessionStartRequestRejected : public TxMessageBase
{
public:

	enum class Reason
	{
		SessionIsAlreadyActive = 0x1,
	};

	//ctor
	SessionStartRequestRejected(Reason reason) 
	{
		type = MessageType::SessionStartRequestRejected;
		AddToPayload((int)reason);
	}

private:
};
