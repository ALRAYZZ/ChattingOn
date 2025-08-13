#pragma once

#include <string>

namespace ChattingOn
{
	struct Config
	{
		std::string serverAddress;
		uint16_t controlPort;
		uint16_t audioPort;
		int sampleRate;
		int channels;
		int frameSizeMs;

		static Config Load(const std::string& configFile);
	};
}