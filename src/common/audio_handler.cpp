#include "audio_handler.h"
#include <spdlog/spdlog.h>
#include <stdexcept>

namespace ChattingOn
{
	AudioHandler::AudioHandler(const Config& config, AudioCallback captureCb, PlaybackCallback playbackCb)
		: config(config), captureCb(std::move(captureCb)), playbackCb(std::move(playbackCb)), stream(nullptr)
	{
		PaError err = Pa_Initialize();
		if (err != paNoError)
		{
			throw std::runtime_error(std::string("PortAudio init failed: ") + Pa_GetErrorText(err));
		}

		err = Pa_OpenDefaultStream(&stream, config.channels, config.channels, paInt16, config.sampleRate, config.frameSizeMs * config.sampleRate / 1000,
									PaCallback, this);
		if (err != paNoError)
		{
			Pa_Terminate();
			throw std::runtime_error(std::string("PortAudio open stream failed: ") + Pa_GetErrorText(err));
		}
	}

	AudioHandler::~AudioHandler()
	{
		Stop();
		Pa_Terminate();
	}

	void AudioHandler::Start()
	{
		if (stream)
		{
			PaError errr = Pa_StartStream(stream);
			if (err != paNoError)
			{
				throw std::runtime_error(std::string("PortAudio start stream failed: ") + Pa_GetErrorText(err));
			}
			spdlog::info("Audio stream started");
		}
	}

	void AudioHandler::Stop()
	{
		if (stream)
		{
			PaError err = Pa_StopStream(stream);
			if (err != paNoError)
			{
				spdlog::warn("PortAudio stop stream failed: {}", Pa_GetErrorText(err));
			}
			err = Pa_CloseStream(stream);
			if (err != paNoError)
			{
				spdlog::warn("PortAudio close stream failed: {}", Pa_GetErrorText(err));
			}
			stream = nullptr;
			spdlog::info("Audio stream stopped");
		}
	}

	int AudioHandler::PaCallback(const void* inputBuffer, void* outputBuffer, unsigned long frameCount,
		const PaStreamCallbackTimeInfo* /*timeInfo*/, PaStreamCallbackFlags /*statusFlags*/, void* userData)
	{
		auto* handler = static_cast<AudioHandler*>(userData);
		if (input)
		{
			std::vector<char> data(reinterpret_cast<const char*>(inputBuffer), reinterpret_cast<const char*>(inputBuffer)
				+ frameCount * 2); // 2 bytes per sample for int16
			handler->captureCb(data);
		}
		if (outputBuffer)
		{
			auto data = handler->playbackCb();
			if (data.size() >= frameCount * 2) // Ensure enough data for output
			{
				std::copy(data.begin(), data.begin() + frameCount * 2, reinterpret_cast<char*>(outputBuffer));
			}
			else
			{
				std::fill_n(reinterpret_cast<char*>(outputBuffer), frameCount * 2, 0); // Fill with silence if not enough data
			}
		}
		return paContinue;
	}
}