#include "jitter_buffer.h"
#include <algorithm>

namespace ChattingOn
{
	JitterBuffer::JitterBuffer(size_t maxFrames) : maxFrames(maxFrames), nextSequence(0)
	{
	}

	void JitterBuffer::AddPacket(const AudioPAcket& packet, uint32_t sequenceNumber)
	{
		if (buffer.size() >= maxFrames)
		{
			buffer.pop_front(); // Remove oldest packet if buffer is full
		}
		buffer.push_back({ packet, sequenceNumber });
		std::sort(buffer.begin(), buffer.end(),
			[](const BufferedPacket& a, const BufferedPacket& b) {return a.sequenceNumber < b.sequenceNumber; });
	}

	std::vector<char> JitterBuffer::GetNextFrame()
	{
		if (buffer.empty())
		{
			return std::vector<char>(960 * 2, 0); // Silence
		}
		auto packet = buffer.front();
		if (packet.sequenceNumber < nextSequence)
		{
			buffer.pop_front();
			return std::vector<char>(960 * 2, 0);
		}
		if (packet.sequenceNumber > nextSequence)
		{
			return std::vector<char>(960 * 2, 0);
		}
		buffer.pop_front();
		nextSequence++;
		return packet.packet.audioData;
		
	}
}