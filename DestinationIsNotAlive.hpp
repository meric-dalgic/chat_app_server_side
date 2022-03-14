#pragma once
#include "TxMessageBase.hpp"


		class DestinationIsNotAlive : public TxMessageBase
		{
		public:

			//ctor
			DestinationIsNotAlive(const std::string& msgContent) : m_msgContent(msgContent)
			{
				type = MessageType::DestinationIsNotAlive;
				AddToPayload(m_msgContent);
			}

		private:
			std::string m_msgContent{};
		};
