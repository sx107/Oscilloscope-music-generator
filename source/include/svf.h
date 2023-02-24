#pragma once
#include <string>
#include <vector>
#include "point.h"

class svfFile {
	public:
		svfFile();
		svfFile(std::string fname);
		bool open(std::string fname);
		std::vector<std::vector<oscMusic::point>> data;
};