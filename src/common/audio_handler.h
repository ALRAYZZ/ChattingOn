#pragma once

#include "audio_packet.h"
#include "network_utils.h"
#include <portaudio.h>
#include <functional>
#include <vector>

namespace ChattingOn
{
	class AudioHandler
	{
	public:
		using AudioCallback = std::function<void(const std::vector<char>&)>;
		using PlaybackCallback = std::function<std::vector<char>()>;

		AudioHandler(const Config& config, AudioCallback captureCb, PlaybackCallback playbackCb);
		~AudioHandler();
		
		void Start();
		void Stop();
		bool IsActive() const { return stream != nullptr; }

	private:
		static int PaCallback(const void* inputBuffer, void* outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo,
			PaStreamCallbackFlags statusFlags, void* userData);

		Config config;
		AudioCallback captureCb;
		PlaybackCallback playbackCb;
		PaStream* stream;

	};
}
