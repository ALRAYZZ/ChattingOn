#include "audio_packet.h"
#include <nlohmann/json.hpp>

namespace ChattingOn
{
	std::vector<char> AudioPacket::Serialize() const
	{
		nlohmann::json j;
		j["clientId"] = clientId;
		j["roomId"] = roomId;
		j["sqeuenceNumber"] = sequenceNumber;
		std::string header = j.dump(); // Serialize the json object to a string
		std::vector<char> result(header.begin(), header.end()); // Using Vector we can mix text + binary data. Its resizable like char* array on C.
		result.push_back('\0');
		result.insert(result.end(), audioData.begin(), audioData.end());
		return result;
	}

	AudioPacket AudioPacket::Deserialize(const std::vector<char>& data)
	{
		AudioPacket packet;
		auto sep = std::find(data.begin(), data.end(), '\0');
		if (sep == data.end()) // Memory address comparison
		{
			throw std::runtime_error("Invalid audio packet: no separator");
		}

		std::string header(data.begin(), sep);
		nlohmann::json j = nlohmann::json::parse(header);
		packet.clientId = j["clientId"].get<std::string>();
		packet.roomId = j["roomId"].get<std::string>();
		packet.sequenceNumber = j["sequenceNumber"].get<uint32_t>();
		packet.audioData = std::vector<char>(sep + 1, data.end());
		return packet;
	}
}