#include "room_manager.h"

namespace ChattingOn
{
	void RoomManager::AddClientToRoom(const std::string& clientId, const std::string& roomId, const udp::endpoint& audioEndpoint)
	{
		rooms[roomId][clientId] = ClientInfo{ roomId, audioEndpoint }; // Creating ClientInfo object and assigning it directly
	}

	void RoomManager::RemoveClientFromRoom(const std::string& clientId, const std::string& roomId)
	{
		if (auto roomIt = rooms.find(roomId); roomIt != rooms.end()) // If roomIt points to a valid room it wont point to end()
		{
			roomIt->second.erase(clientId);
			if (roomIt->second.empty())
			{
				rooms.erase(roomIt);
			}
		}
	}

	std::vector<udp::endpoint> RoomManager::GetAudioEndpointsInRoom(const std::string& roomId, const std::string& excludeClientId) const
	{
		std::vector<udp::endpoint> endpoints;
		if (auto roomIt = rooms.find(roomId); roomIt != rooms.end())
		{
			for (const auto& [clientId, info] : roomIt->second)
			{
				if (clientId != excludeClientId)
				{
					endpoints.push_back(info.audioEndpoint);
				}
			}
		}
		return endpoints;
	}
}
