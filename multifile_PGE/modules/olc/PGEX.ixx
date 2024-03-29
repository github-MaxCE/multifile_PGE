export module olc.PGEX;

namespace olc
{
	class PixelGameEngine;
	// O------------------------------------------------------------------------------O
	// | PGE EXTENSION BASE CLASS - Permits access to PGE functions from extension    |
	// O------------------------------------------------------------------------------O
	export class PGEX
	{
		friend class olc::PixelGameEngine;
	public:
		PGEX(bool bHook = false);

	protected:
		virtual void OnBeforeUserCreate() { }
		virtual void OnAfterUserCreate() { }
		virtual bool OnBeforeUserUpdate(float& fElapsedTime) { return false; }
		virtual void OnAfterUserUpdate(float fElapsedTime) { }

	protected:
		static PixelGameEngine* pge;
	};

	olc::PixelGameEngine* olc::PGEX::pge = nullptr;
}