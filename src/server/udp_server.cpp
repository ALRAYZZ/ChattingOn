#include "udp_server.h"
#include <spdlog/spdlog.h>

namespace ChattingOn
{
	UdpServer::UdpServer(boost::asio::io_context& ioContext, uint16_t port, PacketHandler packetHandler)
		: ioContext(ioContext), socket(ioContext, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port)), packetHandler(std::move(packetHandler))
	{
	}

	void UdpServer::Start()
	{
		StartReceive();
	}

	void UdpServer::SendPacket(const std::vector<char>& packet, const boost::asio::ip::udp::endpoint& destEndpoint)
	{
		socket.async_send_to(boost::asio::buffer(packet), destEndpoint,
			[](const boost::system::error_code& error, std::size_t /*bytes*/)
			{
				if (error)
				{
					spdlog::error("UDP send error: {}", error.message());
				}
			});
	}

	void UdpServer::StartReceive()
	{ 
		socket.async_receive_from(boost::asio::buffer(receiveBuffer), senderEndpoint,
			[this](const boost::system::error_code& error, std::size_t bytes)
			{
				if (!error)
				{
					std::vector<char> data(receiveBuffer.begin(), receiveBuffer.begin() + bytes);
					packetHandler(data, senderEndpoint);
				}
				else
				{
					spdlog::error("UDP receive error: {}", error.message());
				}
				StartReceive(); // Continue receiving
			});
	}
}