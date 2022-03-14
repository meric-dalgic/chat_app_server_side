#pragma once
#include "TxMessageBase.hpp"


class MessageForwardingToDestUser : public TxMessageBase
{
public:

	//ctor
	MessageForwardingToDestUser(std::string sender_user_name ,const std::string& msgContent) :
	m_sender_user_name(sender_user_name), m_msgContent(msgContent)
	{
		type = MessageType::MessageForwardingToDestUser;
		AddToPayload(m_sender_user_name);
		AddToPayload(m_msgContent);
	}

private:
	std::string m_sender_user_name{};
	std::string m_msgContent{};
};
