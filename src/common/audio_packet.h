#pragma once

#include <string>
#include <vector>

namespace ChattingOn
{
	struct AudioPacket
	{
		std::string clientId;
		std::string roomId;
		uint32_t sequenceNumber;
		std::vector<char> audioData;

		std::vector<char>Serialize() const;
		static AudioPacket Deserialize(const std::vector<char>& data);
	};
}