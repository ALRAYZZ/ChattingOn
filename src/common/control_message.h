#pragma once

#include <nlohmann/json.hpp>
#include <string>

namespace ChattingOn
{
	using json = nlohmann::json;

	class ControlMessage
	{

	public:
		enum class Type { JOIN_ROOM, LEAVE_ROOM, HEARTBEAT };

		ControlMessage(Type type, std::string clientId, std::string roomId)
			: type(type), clientId(std::move(clientId)), roomId(std::move(roomId)) {
		}

		std::string Serialize() const;
		static ControlMessage Deserialize(const std::string& data);

		Type GetType() const { return type; }
		const std::string& GetClientId() const { return clientId; }
		const std::string& GetRoomId() const { return roomId; }

	private:
		Type type;
		std::string clientId;
		std::string roomId;
	};
}