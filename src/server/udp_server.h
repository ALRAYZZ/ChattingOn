#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <vector>

namespace ChattingOn
{
	class UdpServer
	{
	public:
		using PacketHandler = std::function<void(const std::vector<char>&, const boost::asio::ip::udp::endpoint&)>;

		UdpServer(boost::asio::io_context& ioContext, uint16_t port, PacketHandler handler);
		void Start();
		void SendPacket(const std::vector<char>& packet, const boost::asio::ip::udp::endpoint& destEndpoint);

	private:
		void StartReceive();

		boost::asio::io_context& ioContext;
		boost::asio::ip::udp::socket socket;
		PacketHandler packetHandler;
		std::array<char, 1024> receiveBuffer;
		boost::asio::ip::udp::endpoint senderEndpoint;
	};
}