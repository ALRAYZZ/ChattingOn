#pragma once

#include "audio_handler.h"
#include "jitter_buffer.h"
#include "network_utils.h"
#include "tcp_client.h"
#include "udp_client.h"
#include <boost/asio.hpp>
#include <windows.h>

namespace ChattingOn
{
	class Client
	{
	public:
		Client(boost::asio::io_context& ioContext, const Config& config);
		~Client();
		void Start();
		void Stop();
		void JoinRoom(const std::string& clientId, const std::string& roomId);
		void TogglePushToTalk(bool enable);
		bool IsCapturing() const;

	private:
		void HandleControlMessage(const ControlMessage& msg);
		void HandleUdpPacket(const std::vector<char>& data);
		void HandleAudioCapture(const std::vector<char>& data);
		std::vector<char> HandleAudioPlayback();

		boost::asio::io_context& ioContext;
		Config config;
		TcpClient tcpClient;
		UdpClient udpClient;
		AudioHandler audioHandler;
		JitterBuffer jitterBuffer;
		boost::asio::ip::udp::endpoint serverEndpoint;
		std::string ClientId;
		std::string RoomId;
		uint32_t sequenceNumber;
		bool pushToTalkEnabled;
		bool isCapturing;
	};
}