#pragma once

#include <boost/asio.hpp>
#include <functional>
#include <vector>

namespace ChattingOn
{
	class UdpClient
	{
	public:
		using PacketHandler = std::function<void(const std::vector<char>&)>;

		UdpClient(boost::asio::io_context& ioContext, uint16_t port, PacketHandler packetHandler);
		void Start();
		void SendPacket(const std::vector<char>& data, const boost::asio::ip::udp::endpoint& endpointDestination);

	private:
		void StartReceive();

		boost::asio::io_context& ioContext;
		boost::asio::ip::udp::socket socket;
		PacketHandler packetHandler;
		std::array<char, 1200> buffer;
	};
}