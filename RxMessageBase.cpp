#pragma once
#include "RxMessageBase.hpp"

#include <string>
#include <vector>

#include "ClientMessageWasCorrupted.hpp"
#include "IAmOnline.hpp"
#include "MessageToAll.hpp"
#include "MessageToUser.hpp"
#include "SessionStartRequest.hpp"
#include "WhoIsOnline.hpp"


RxMessageBase* RxMessageBase::CreateInstance(MessageType type)
{
	switch (type)
	{
	case RxMessageBase::MessageType::SessionStartRequest:
		return new SessionStartRequest;

	case RxMessageBase::MessageType::MessageToUser:
		return new MessageToUser;

	case RxMessageBase::MessageType::MessageToAll:
		return new MessageToAll;

	case RxMessageBase::MessageType::WhoIsOnline:
		return new WhoIsOnline;

	case RxMessageBase::MessageType::IAmOnline:
		return new IAmOnline;

		//case RxMessageBase::MessageType::ClientMessageWasCorrupted:
		//	return new ClientMessageWasCorrupted;

	default:
		throw std::exception("unknown type");
	}
}

bool RxMessageBase::SessionIsAlreadyActive()
{
	/*return Executer::IsSessionAlreadyAlive(m_endpoint, userName);*/

	if (Executer::ActiveSessionList.size() == 0)
	{
		return false;
	}

	for (auto& session : Executer::ActiveSessionList)
	{
		if (session->m_username == userName)
		{
			return true;
		}
	}

	return false;
}

Session* RxMessageBase::FindSession(std::string desired_user_name)
{
	desired_user_name.erase(remove(desired_user_name.begin(), desired_user_name.end(), '\0'), desired_user_name.end());


	if (Executer::ActiveSessionList.size() == 0)
	{
		return nullptr;
	}

	for (auto& session : Executer::ActiveSessionList)
	{
		if (session->m_username.compare(desired_user_name) == 0)
		{
			return session;
		}
	}

	return nullptr;
}


