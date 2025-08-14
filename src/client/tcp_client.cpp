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
		data += '\n'; // Ensure newline termination for message parsing
		
		auto dataPtr = std::make_shared<std::string>(std::move(data)); // Using a pointer so that the data remains valid during async operation
		boost::asio::async_write(socket, boost::asio::buffer(*dataPtr),
			[this, dataPtr](const boost::system::error_code& error, std::size_t /*bytes*/)
			{
				if (error)
				{
					spdlog::error("Write error: {}", error.message());
				}
			});
	}

	void TcpClient::StartRead()
	{
		boost::asio::async_read_until(socket, receiveBuffer, '\n',
			[this](const boost::system::error_code& error, std::size_t /*bytes*/)
			{
				if (!error)
				{
					std::istream stream(&receiveBuffer);
					std::string line;
					std::getline(stream, line);

					// Remove any trailing
					if (!line.empty() && line.back() == '\r')
					{
						line.pop_back();
					}

					if (!line.empty())
					{
						try
						{
							auto msg = ControlMessage::Deserialize(line);
							messageHandler(msg);
						}
						catch (const std::exception& ex)
						{
							spdlog::error("Parse error: {}", ex.what());
						}
					}
					StartRead(); // Continue reading more messages
				}
				else
				{
					spdlog::error("Read error: {}", error.message());
				}
			});
	}
}