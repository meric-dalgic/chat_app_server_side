#pragma once
#include "TxMessageBase.hpp"


		class ClientMessageWasCorrupted : public TxMessageBase
		{
		public:

			//ctor
			ClientMessageWasCorrupted(const std::string& msgContent) : m_msgContent(msgContent)
			{
				type = MessageType::ClientMessageWasCorrupted;
				AddToPayload(m_msgContent);
			}

		private:
			std::string m_msgContent{};
		};
