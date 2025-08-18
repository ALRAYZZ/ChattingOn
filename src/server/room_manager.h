#pragma once

#include <boost/asio.hpp>
#include <map>
#include <set>
#include <string>
#include <unordered_map>

namespace ChattingOn
{
	using boost::asio::ip::udp;

	class RoomManager
	{
	public:
		void AddClientToRoom(const std::string& clientId, const std::string& roomId, const udp::endpoint& audioEndpoint);
		void RemoveClientFromRoom(const std::string& clientId, const std::string& roomId);
		std::vector<udp::endpoint> GetAudioEndpointsInRoom(const std::string& roomId, const std::string& excludeClientId) const;

	private:
		struct ClientInfo
		{
			std::string roomId;
			udp::endpoint audioEndpoint;
		};
		std::map<std::string, std::unordered_map<std::string, ClientInfo>> rooms; // roomId -> clientId -> ClientInfo
	};
}