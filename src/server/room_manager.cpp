#include "room_manager.h"

namespace ChattingOn
{
	const  std::set<std::string> RoomManager::emptySet = {};

	void RoomManager::AddClientToRoom(const std::string& clientId, const std::string& roomId)
	{
		rooms[roomId].insert(clientId);
	}

	void RoomManager::RemoveClientFromRoom(const std::string& clientId, const std::string& roomId)
	{
		if (auto it = rooms.find(roomId); it != rooms.end())
		{
			it->second.erase(clientId);
			if (it->second.empty())
			{
				rooms.erase(it);
			}
		}
	}

	const std::set<std::string>& RoomManager::GetClientsInRoom(const std::string& roomId) const
	{
		if (auto it = rooms.find(roomId); it != rooms.end())
		{
			return it->second;
		}
		return emptySet;
	}
}
