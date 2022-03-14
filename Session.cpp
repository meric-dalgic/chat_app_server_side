#include "Executer.hpp"
#include "Session.hpp"

Session* Session::FindRelatedSession(std::string desired_user_name)
{
	desired_user_name.erase(remove(desired_user_name.begin(), desired_user_name.end(), '\0'), desired_user_name.end());


	if (Executer::ActiveSessionList.size() == 0)
	{
		return nullptr;
	}

	for (auto session : Executer::ActiveSessionList)
	{
		if (session->m_username.compare(desired_user_name) == 0)
		{
			return session;
		}
	}

	return nullptr;
}

bool Session::RemoveRelatedSession(std::string desired_user_name)
{
	desired_user_name.erase(remove(desired_user_name.begin(), desired_user_name.end(), '\0'), desired_user_name.end());


	if (Executer::ActiveSessionList.size() == 0)
	{
		return false;
	}


	for (auto iter{ Executer::ActiveSessionList.begin()}; iter!= Executer::ActiveSessionList.end(); ++iter)
	{
		if ((*iter)->m_username.compare(desired_user_name) == 0)
		{
			//std::cout << "Active session before removing: " << Executer::ActiveSessionList.size() << "\n";
			Executer::ActiveSessionList.erase(iter);
			//delete *iter;
			//std::cout << "Active session after removing: " << Executer::ActiveSessionList.size() << "\n";
			break;
		}
	}

	return false;
}
