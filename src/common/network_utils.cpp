#include "network_utils.h"
#include <fstream>
#include <nlohmann/json.hpp>

namespace ChattingOn
{
	Config Config::Load(const std::string& configFile)
	{
		Config cfg;
		std::ifstream ifs(configFile);
		nlohmann::json j = nlohmann::json::parse(ifs);
		cfg.serverAddress = j["serverAddress"].get<std::string>();
		cfg.controlPort = j["controlPort"].get<uint16_t>();
		cfg.audioPort = j["audioPort"].get<uint16_t>();
		cfg.sampleRate = j["sampleRate"].get<int>();
		cfg.channels = j["channels"].get<int>();
		cfg.frameSizeMs = j["frameSizeMs"].get<int>();

		return cfg;
	}
}