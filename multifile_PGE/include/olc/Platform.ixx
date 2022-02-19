export module olc.Platform;

import std.core;
import olc.PGE;
import olc.xtra;
import olc.V2D;

namespace olc
{
	export class Platform
	{
	public:
		virtual ~Platform() = default;
		virtual olc::rcode ApplicationStartUp() = 0;
		virtual olc::rcode ApplicationCleanUp() = 0;
		virtual olc::rcode ThreadStartUp() = 0;
		virtual olc::rcode ThreadCleanUp() = 0;
		virtual olc::rcode CreateGraphics(bool bFullScreen, bool bEnableVSYNC, const olc::vi2d& vViewPos, const olc::vi2d& vViewSize) = 0;
		virtual olc::rcode CreateWindowPane(const olc::vi2d& vWindowPos, olc::vi2d& vWindowSize, bool bFullScreen) = 0;
		virtual olc::rcode SetWindowTitle(const std::string& s) = 0;
		virtual olc::rcode StartSystemEventLoop() = 0;
		virtual olc::rcode HandleSystemEvent() = 0;
		static olc::PixelGameEngine* ptrPGE;
	};

	olc::PixelGameEngine* olc::Platform::ptrPGE = nullptr;
}