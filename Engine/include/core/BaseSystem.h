#pragma once

namespace oasis
{
	namespace core
	{
		class BaseSystem
		{
		public:
			virtual bool Initialize(int nArgs, ...) = 0;
			virtual bool Destroy() = 0;
		};
	}
}