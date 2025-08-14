#pragma once

#include "control_message.h"
#include <boost/asio.hpp>
#include <functional>
#include <memory>

namespace ChattingOn
{
	class TcpClient
	{
	public:
		using MessageHandler = std::function<void(const ControlMessage&)>;

		TcpClient(boost::asio::io_context& ioContext, const std::string& serverAddress, uint16_t port,
			MessageHandler messageHandler);

		void Start();
		void SendControlMessage(const ControlMessage& message);

	private:
		void StartRead();

		boost::asio::io_context& ioContext;
		boost::asio::ip::tcp::socket socket;
		MessageHandler messageHandler;
		boost::asio::streambuf receiveBuffer;
	};
}