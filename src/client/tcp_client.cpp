#include "tcp_client.h"
#include <spdlog/spdlog.h>

namespace ChattingOn
{
	TcpClient::TcpClient(boost::asio::io_context& ioContext, const std::string& serverAddress, uint16_t port, MessageHandler messageHandler)
		: ioContext(ioContext), socket(ioContext), messageHandler(std::move(messageHandler))
	{
		socket.async_connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::make_address(serverAddress), port),
			[this](const boost::system::error_code& error)
			{
				if (!error)
				{
					spdlog::info("Connected to server");
					StartRead();
				}
				else
				{
					spdlog::error("Connect error: {}", error.message());
				}
			});
	}

	void TcpClient::Start()
	{
		//  Connection started in constructor, no need to do anything here.
	}

	void TcpClient::SendControlMessage(const ControlMessage& message)
	{
		std::string data = message.Serialize();
		socket.async_write_some(boost::asio::buffer(data), [](const boost::system::error_code& error, std::size_t /*bytes*/)
			{
				if (error)
				{
					spdlog::error("Write error: {}", error.message());
				}
			});
	}

	void TcpClient::StartRead()
	{
		socket.async_read_some(boost::asio::buffer(readBuffer),
			[this](const boost::system::error_code& error, std::size_t bytes)
			{
				if (!error)
				{
					std::string data(readBuffer.data(), bytes);
					try
					{
						auto msg = ControlMessage::Deserialize(data);
						messageHandler(msg);
						StartRead();
					}
					catch (const std::exception& ex)
					{
						spdlog::error("Parse error: {}", ex.what());
					}
				}
				else
				{
					spdlog::error("Read error: {}", error.message());
				}
			});
	}
}