#include "udp_client.h"
#include <spdlog/spdlog.h>

namespace ChattingOn
{
	UdpClient::UdpClient(boost::asio::io_context& ioContext, uint16_t port, PacketHandler packetHandler)
		: ioContext(ioContext), socket(ioContext, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0)),
		packetHandler(std::move(packetHandler)) {}

	void UdpClient::Start()
	{
		StartReceive();
	}

	void UdpClient::SendPacket(const std::vector<char>& data, const boost::asio::ip::udp::endpoint& endpointDestination)
	{
		socket.async_send_to(boost::asio::buffer(data), endpointDestination,
			[](const boost::system::error_code& error, std::size_t /*bytes*/)
			{
				if (error)
				{
					spdlog::error("UDP send error: {}", error.message());
				}
			});
	}

	void UdpClient::StartReceive()
	{
		socket.async_receive(boost::asio::buffer(buffer),
			[this](const boost::system::error_code& error, std::size_t bytes)
			{
				if (!error)
				{
					std::vector<char> data(buffer.begin(), buffer.begin() + bytes);
					packetHandler(data);
				}
				else
				{
					spdlog::error("UDP receive error: {}", error.message());
				}
				StartReceive();
			});
	}
}