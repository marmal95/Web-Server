#pragma once

namespace web
{
	class IServer
	{
	public:
		virtual void start() = 0;
		virtual void stop() = 0;
		virtual void reset() = 0;
		virtual ~IServer() = default;
	};
}