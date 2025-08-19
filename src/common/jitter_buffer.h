#pragma once
#include "audio_packet.h"
#include <deque>
#include <cstdint>

namespace ChattingOn
{
	class JitterBuffer
	{
	public:
		explicit JitterBuffer(size_t maxFrames = 5); // 100ms at 20ms/frame
		void AddPacket(const AudioPacket& packet, uint32_t sequenceNumber);
		std::vector<char> GetNextFrame();
		bool HashData() const {return !buffer.empty(); }

	private:
		struct BufferedPAcket
		{
			AudioPacket packet;
			uint32_t sequenceNumber;
		};
		std::deque<BufferedPacket> buffer;
		size_t maxFrames;
		uint32_t nextSequence;
	};
}