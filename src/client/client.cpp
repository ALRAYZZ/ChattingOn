#include "client.h"
#include <spdlog/spdlog.h>

namespace ChattingOn
{
	Client::Client(boost::asio::io_context& ioContext, const Config& config)
		: ioContext(ioContext), config(config),
		tcpClient(ioContext, config.serverAddress, config.controlPort,
			[this](const ControlMessage& msg) { HandleControlMessage(msg); }),
		udpClient(ioContext, config.audioPort,
			[this](const std::vector<char>& data) { HandleUdpPacket(data); }) {}

	void Client::Start()
	{
		spdlog::info("Starting ChattingOn client to {}:{}", config.serverAddress, config.controlPort);
		tcpClient.Start();
		udpClient.Start();
	}

	void Client::JoinRoom(const std::string& clientId, const std::string& roomId)
	{
		ControlMessage msg(ControlMessage::Type::JOIN_ROOM, clientId, roomId);
		tcpClient.SendControlMessage(msg);
	}

	void Client::HandleControlMessage(const ControlMessage& msg)
	{
		spdlog::info("Received control: {} f or room {}", static_cast<int>(msg.GetType()), msg.GetRoomId());
	}

	void Client::HandleUdpPacket(const std::vector<char>& data)
	{
		spdlog::info("Received UDP packet of size {}", data.size());
		// Process audio data here
	}
}