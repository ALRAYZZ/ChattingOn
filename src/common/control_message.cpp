#include <control_message.h>

namespace ChattingOn
{
	std::string ControlMessage::Serialize() const
	{
		json j;
		j["type"] = static_cast<int>(type);
		j["clientId"] = clientId;
		j["roomId"] = roomId;
		return j.dump();
	}

	ControlMessage ControlMessage::Deserialize(const std::string& data)
	{
		json j = json::parse(data);
		return ControlMessage(
			static_cast<Type>(j["type"].get<int>()),
			j["clientId"].get<std::string>(),
			j["roomId"].get<std::string>());
	}
}