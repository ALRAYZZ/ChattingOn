#pragma once

#include "control_message.h"
#include <boost/asio.hpp>
#include <functional>
#include <memory>
#include <string>

namespace ChattingOn
{
	class TcpServer
	{
	public:
		using MessageHandler = std::function<void(const ControlMessage&, const std::string& clientId)>;

		TcpServer(boost::asio::io_context& ioContext, uint16_t port, MessageHandler handler);
		void Start();

	private:
		void StartAccept();
		void StartRead(std::shared_ptr<boost::asio::ip::tcp::socket> socket, const std::string& clientId);

		boost::asio::io_context& ioContext;
		boost::asio::ip::tcp::acceptor acceptor;
		MessageHandler messageHandler;

	};
}