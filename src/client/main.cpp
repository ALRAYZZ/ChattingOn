#include "client.h"
#include <spdlog/spdlog.h>
#include <windows.h>

int main()
{
	try
	{
		ChattingOn::Config config = ChattingOn::Config::Load("config/config.json");
		boost::asio::io_context ioContext;
		ChattingOn::Client client(ioContext, config);
		client.Start();
		client.JoinRoom("client1", "room1");

		// Register PTT hotkey (Ctrl)
		RegisterHotKey(NULL, 1, MOD_CONTROL, VK_CONTROL);

		MSG msg;
		bool ctrlPressed = false;

		while (GetMessage(&msg, NULL, 0, 0))
		{
			if (msg.message == WM_HOTKEY)
			{
				// Check if Ctrl is pressed
				bool isCtrlDown = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;

				if (isCtrlDown && !ctrlPressed)
				{
					// Ctrl just pressed - start capturing
					client.TogglePushToTalk(true);
					ctrlPressed = true;
					spdlog::info("Push to talk enabled");
				}
				else if (!isCtrlDown && ctrlPressed)
				{
					// Ctrl just released - stop capturing
					client.TogglePushToTalk(false);
					ctrlPressed = false;
					spdlog::info("Push to talk disabled");
				}
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		UnregisterHotKey(NULL, 1);
		client.Stop();
	}
	catch (const std::exception& ex)
	{
		spdlog::error("Client error: {}", ex.what());
		return 1;
	}
	return 0;
}