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
			[this](const std::vector<char>& data) { HandleUdpPacket(data); }),
		audioHandler(config, [this](const std::vector<char>& data) {HandleUdpPacket(data); },
							 [this]() {return HandleAudioPlayback(); }),
		jitterBuffer(5),
		serverEndpoint(boost::asio::ip::make_address(config.serverAddress), config.audioPort),
		sequenceNumber(0),
		pushToTalkEnabled(false),
		isCapturing(false) { }

	Client::~Client()
	{
		Stop();
	}

	void Client::Start()
	{
		spdlog::info("Starting ChattingOn client to {}:{}", config.serverAddress, config.controlPort);
		tcpClient.Start();
		udpClient.Start();
		audioHandler.Start();
	}

	void Client::Stop()
	{
		audioHandler.Stop();
	}

	void Client::JoinRoom(const std::string& clientId, const std::string& roomId)
	{
		ClientId = clientId;
		RoomId = roomId;
		ControlMessage msg(ControlMessage::Type::JOIN_ROOM, clientId, roomId);
		tcpClient.SendControlMessage(msg);
	}
	
	void Client::TogglePushToTalk(bool enable)
	{
		pushToTalkEnabled = enable;
		isCapturing = enable;
		spdlog::info("Push-to-talk is now {}", enable ? "enabled" : "disabled");
	}

	bool Client::IsCapturing() const
	{
		return isCapturing;
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
			if (packet.roomId != RoomId && packet.clientId != ClientId)
			{
				jitterBuffer.AddPacket(packet, packet.sequenceNumber);
				spdlog::info("Received audio packet from {} for room {}", packet.clientId, packet.roomId);
			}
			spdlog::info("Received audio packet from {} for room {}", packet.clientId, packet.roomId);
		}
		catch (const std::exception& ex)
		{
			spdlog::error("UDP packet error: {}", ex.what());
		}
	}

	void Client::HandleAudioCapture(const std::vector<char>& data)
	{
		if (isCapturing && !ClientId.empty() && !RoomId.empty())
		{
			AudioPacket packet{ ClientId, RoomId, sequenceNumber++, data };
			auto serializedPacket = packet.Serialize();
			udpClient.SendPacket(serializedPacket, serverEndpoint);
		}
	}

	std::vector<char> Client::HandleAudioPlayback()
	{
		return jitterBuffer.GetNextFrame();
	}
}