#pragma once
#include "simpleGenerator.h"
#include "svf.h"
#include <string>
#include <dirent.h>
#include "boost/filesystem.hpp"
#include <vector>

class textGenerator {
	public:
		enum class hOrigin
		{
			left,
			right,
			center
		};
	
		textGenerator();
		textGenerator(std::string font);
		~textGenerator();
		bool loadFont(const boost::filesystem::path dname);
		void drawText(double freq, std::string text, hOrigin origin, oscMusic::point pos, double scale = 1, double angle = 0, double completion = 1);
		double hspacing;
		double spaceWidth;
		
	private:
		svfFile* font[256];
		double l_leftX[256];
		double l_rightX[256];
		void init();
};