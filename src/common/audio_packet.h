#pragma once

#include <string>
#include <vector>

namespace ChattingOn
{
	struct AudioPacket
	{
		std::string clientId;
		std::string roomId;
		std::vector<char> audioData;

		std::vector<char>Serialize() const;
		static AudioPacket Deserialize(const std::vector<char>& data);
	};
}