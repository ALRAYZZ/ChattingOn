#include "client.h"
#include <spdlog/spdlog.h>
#include "audio_packet.h"

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
	
	void Client::SendTestAudioPacket(const std::string& clientId, const std::string& roomId)
	{
		AudioPacket packet(clientId, roomId, std::vector<char>(960 * 2, 0)); // 20 ms 48kHz stereo audio, PCM16
		auto data = packet.Serialize();
		udpClient.SendPacket(data, serverEndpoint);
		spdlog::info("Sent test audio packet to room {}", roomId);
	}

	void Client::HandleControlMessage(const ControlMessage& msg)
	{
		spdlog::info("Received control: {} f or room {}", static_cast<int>(msg.GetType()), msg.GetRoomId());
	}

	void Client::HandleUdpPacket(const std::vector<char>& data)
	{
		try
		{
			auto packet = AudioPacket::Deserialize(data);
			spdlog::info("Received audio packet from {} for room {}", packet.clientId, packet.roomId);
		}
		catch (const std::exception& ex)
		{
			spdlog::error("UDP packet error: {}", ex.what());
		}
	}
}