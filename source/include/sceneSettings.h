#pragma once
#include <cstdint>
#include "genericBuffer.h"

namespace oscMusic {
	class sceneSettings {
		public:
			static uint32_t sampleRate;
			static oscMusic::genericBuffer* buffer;
	};
}

