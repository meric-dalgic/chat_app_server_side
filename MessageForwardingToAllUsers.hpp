#pragma once
#include "TxMessageBase.hpp"


		class MessageForwardingToAllUsers : public TxMessageBase
		{
		public:

			//ctor
			MessageForwardingToAllUsers(std::string sender_user_name,const std::string& msgContent) : m_msgContent(msgContent)
			{
				type = MessageType::MessageForwardingToAllUsers;
				AddToPayload(sender_user_name);
				AddToPayload(m_msgContent);
			}

		private:
			std::string m_msgContent{};
		};
