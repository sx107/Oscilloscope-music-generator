#pragma once
namespace oscMusic {
	class genericBuffer {
		public:
			virtual ~genericBuffer() {};
			virtual void putSample(double a, double b) = 0;
	};
}