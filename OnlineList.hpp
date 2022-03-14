#pragma once
#include "TxMessageBase.hpp"


		class OnlineList : public TxMessageBase
		{
		public:

			//ctor
			OnlineList(const std::string& msgContent) : m_msgContent(msgContent)
			{
				type = MessageType::OnlineList;
				AddToPayload(m_msgContent);
			}

		private:
			std::string m_msgContent{};
		};