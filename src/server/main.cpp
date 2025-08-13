#include "server.h"
#include <spdlog/spdlog.h>

int main()
{
	try
	{
		ChattingOn::Config config = ChattingOn::Config::Load("config/config.json");
		boost::asio::io_context ioContext;
		ChattingOn::Server server(ioContext, config);
		server.Start();
		ioContext.run();
	}
	catch (const std::exception& ex)
	{
		spdlog::error("Server error: {}", ex.what());
		return 1;
	}
	return 0;
}