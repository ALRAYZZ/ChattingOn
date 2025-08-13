#pragma once

#include <boost/asio.hpp>
#include <map>
#include <set>
#include <string>

namespace ChattingOn
{
	class RoomManager
	{
	public:
		void AddClientToRoom(const std::string& clientId, const std::string& roomId);
		void RemoveClientFromRoom(const std::string& clientId, const std::string& roomId);
		const std::set<std::string>& GetClientsInRoom(const std::string& roomId) const;

	private:
		std::map<std::string, std::set<std::string>> rooms; // roomId -> {clientId}
		static const std::set<std::string> emptySet; // Used to return an empty set when no clients are in the room
	};
}