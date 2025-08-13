#include "tcp_server.h"
#include <spdlog/spdlog.h>

namespace ChattingOn
{
	TcpServer::TcpServer(boost::asio::io_context& ioContext, uint16_t port, MessageHandler handler)
			: ioContext(ioContext),
			acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)),
			messageHandler(std::move(handler)) { }

	void TcpServer::Start()
	{
		StartAccept();
	}

	void TcpServer::StartAccept()
	{
		auto socket = std::make_shared<boost::asio::ip::tcp::socket>(ioContext);
		acceptor.async_accept(*socket, [this, socket](const boost::system::error_code& error)
			{
				if (!error)
				{
					std::string clientId = socket->remote_endpoint().address().to_string() + ":" +
						std::to_string(socket->remote_endpoint().port());
					spdlog::info("Client connected: {}", clientId);
					StartRead(socket, clientId);
				}
				StartAccept();
			});
	}

	void TcpServer::StartRead(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const std::string& clientId)
	{
		auto buffer = std::make_shared<std::array<char, 1024>>();
		socket->async_read_some(boost::asio::buffer(*buffer),
			[this, socket, clientId, buffer](const boost::system::error_code& error, std::size_t bytes)
			{
				if (!error)
				{
					std::string data(buffer->data(), bytes);
					try
					{
						auto msg = ControlMessage::Deserialize(data);
						messageHandler(msg, clientId);
						StartRead(socket, clientId);
					}
					catch (const std::exception& ex)
					{
						spdlog::error("Parse error grom {}: {}", clientId, ex.what());
					}
				}
				else
				{
					spdlog::error("Read error from {}: {}", clientId, error.message());
				}
			});
	}
}