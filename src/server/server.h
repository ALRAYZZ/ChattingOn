#pragma once

#include "room_manager.h"
#include "tcp_server.h"
#include "udp_server.h"
#include "network_utils.h"
#include <boost/asio.hpp>
#include <unordered_map>

namespace ChattingOn
{
	class Server
	{
	public:
		Server(boost::asio::io_context& ioContext, const Config& config);
		void Start();

	private:
		void HandleControlMessage(const ControlMessage& message, const std::string& clientId);
		void HandleUdpPacket(const std::vector<char>& packet, const boost::asio::ip::udp::endpoint& senderEndpoint);

		boost::asio::io_context& ioContext;
		Config config;
		RoomManager roomManager;
		TcpServer tcpServer;
		UdpServer udpServer;
		std::unordered_map<std::string, udp::endpoint> clientEndpoints; // Maps client IDs to their UDP endpoints
	};
}