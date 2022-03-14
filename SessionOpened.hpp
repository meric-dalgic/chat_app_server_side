#pragma once
#include "TxMessageBase.hpp"


		class SessionOpened : public TxMessageBase
		{
		public:

			//ctor
			SessionOpened(const std::string& msgContent) : m_msgContent(msgContent)
			{
				type = MessageType::SessionOpened;
				AddToPayload(m_msgContent);
			}

		private:
			std::string m_msgContent{};
		};
