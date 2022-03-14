#pragma once
#include "TxMessageBase.hpp"


		class ResponseToIAmOnline : public TxMessageBase
		{
		public:

			//ctor
			ResponseToIAmOnline(const std::string& msgContent) : m_msgContent(msgContent)
			{
				type = MessageType::ResponseToIAmOnline;
				AddToPayload(m_msgContent);
			}

		private:
			std::string m_msgContent{};
		};
