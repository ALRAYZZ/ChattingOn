#include "client.h"
#include <spdlog/spdlog.h>

int main()
{
	try
	{
		ChattingOn::Config config = ChattingOn::Config::Load("config/config.json");
		boost::asio::io_context ioContext;
		ChattingOn::Client client(ioContext, config);
		client.Start();
		client.JoinRoom("client1", "room1");
		ioContext.run();
	}
	catch (const std::exception& ex)
	{
		spdlog::error("Client error: {}", ex.what());
		return 1;
	}
	return 0;
}