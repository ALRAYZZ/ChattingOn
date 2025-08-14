#include "network_utils.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include  <filesystem>

namespace ChattingOn
{
	Config Config::Load(const std::string& configFile)
	{
		Config cfg;

		// DEBUG
		std::filesystem::path currentPath = std::filesystem::current_path();
		std::filesystem::path fullConfigPath = std::filesystem::absolute(configFile);

		// Check file exists
		if (!std::filesystem::exists(configFile))
		{
			throw std::runtime_error("Config file does not exist: " + configFile +
			"\nCurrent working directory: " + currentPath.string() +
			"\nFull path attempted: " + fullConfigPath.string());
		}

		// Check file size
		auto fileSize = std::filesystem::file_size(configFile);
		if (fileSize == 0)
		{
			throw std::runtime_error("Config file is empty: " + configFile);
		}

		std::ifstream ifs(configFile);

		if (!ifs.is_open())
		{
			throw std::runtime_error("Coult not open config file: " + configFile);
		}

		nlohmann::json jsonFile;
		try
		{
			ifs >> jsonFile;
		}
		catch (const nlohmann::json::parse_error& error)
		{
			throw std::runtime_error("JSON parse error in " + configFile + ": " + error.what());
		}

		try
		{
			cfg.serverAddress = jsonFile["serverAddress"].get<std::string>();
			cfg.controlPort = jsonFile["controlPort"].get<uint16_t>();
			cfg.audioPort = jsonFile["audioPort"].get<uint16_t>();
			cfg.sampleRate = jsonFile["sampleRate"].get<int>();
			cfg.channels = jsonFile["channels"].get<int>();
			cfg.frameSizeMs = jsonFile["frameSizeMs"].get<int>();

		}
		catch (const nlohmann::json::out_of_range& error)
		{
			throw std::runtime_error("Missing key in config file " + configFile + ": " + error.what());
		}
		return cfg;
	}
}