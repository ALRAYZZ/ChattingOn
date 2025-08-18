#include "server.h"
#include <spdlog/spdlog.h>
#include "audio_packet.h"

namespace ChattingOn
{
	Server::Server(boost::asio::io_context& ioContext, const Config& config)
		: ioContext(ioContext), config(config),
		tcpServer(ioContext, config.controlPort, [this](const ControlMessage& msg, const std::string& clientId)
			{
				HandleControlMessage(msg, clientId);
			}),
		udpServer(ioContext,config.audioPort, [this](const std::vector<char>& packet, const boost::asio::ip::udp::endpoint& senderEndpoint)
			{
				HandleUdpPacket(packet, senderEndpoint);
			}) {}

	void Server::Start()
	{
		spdlog::info("Starting ChattingOn server on {}:{}", config.serverAddress, config.controlPort);
		tcpServer.Start();
		udpServer.Start();
	}

	void Server::HandleControlMessage(const ControlMessage& msg, const std::string& clientId)
	{
		switch (msg.GetType())
		{
			case ControlMessage::Type::JOIN_ROOM:
				roomManager.AddClientToRoom(clientId, msg.GetRoomId(), clientEndpoints[clientId]);
				spdlog::info("Client {} joined room {}", clientId, msg.GetRoomId());
				break;
				
			case ControlMessage::Type::LEAVE_ROOM:
				roomManager.RemoveClientFromRoom(clientId, msg.GetRoomId());
				spdlog::info("Client {} left room {}", clientId, msg.GetRoomId());
				break;

			case ControlMessage::Type::HEARTBEAT:
				spdlog::info("Heartbeat from {}", clientId);
				break;
		}
	}

	void Server::HandleUdpPacket(const std::vector<char>& data, const boost::asio::ip::udp::endpoint& senderEndpoint)
	{
		try
		{
			auto packet = AudioPacket::Deserialize(data);
			clientEndpoints[packet.clientId] = senderEndpoint; // Update the client's UDP endpoint
			auto endpoints = roomManager.GetAudioEndpointsInRoom(packet.roomId, packet.clientId);
			for (const auto& endpoint : endpoints)
			{
				udpServer.SendPacket(data, endpoint);
			}
			spdlog::info("Forwared audio packet from {} to {} clients in room {}", packet.clientId, endpoints.size(), packet.roomId);
		}
		catch (const std::exception& ex)
		{
			spdlog::error("UDP packet error: {}", ex.what());
		}
	}
}
