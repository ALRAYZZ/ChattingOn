#pragma once

#include "network_utils.h"
#include "tcp_client.h"
#include "udp_client.h"
#include <boost/asio.hpp>

namespace ChattingOn
{
	class Client
	{
	public:
		Client(boost::asio::io_context& ioContext, const Config& config);
		void Start();
		void JoinRoom(const std::string& clientId, const std::string& roomId);
		void SendTestAudioPacket(const std::string& clientId, const std::string& roomId);

	private:
		void HandleControlMessage(const ControlMessage& msg);
		void HandleUdpPacket(const std::vector<char>& data);

		boost::asio::io_context& ioContext;
		Config config;
		TcpClient tcpClient;
		UdpClient udpClient;
		boost::asio::ip::udp::endpoint serverEndpoint;
	};
}