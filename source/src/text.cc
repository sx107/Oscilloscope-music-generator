#include "text.h"
#include <limits>

textGenerator::textGenerator() {
	this->init();
}

textGenerator::textGenerator(std::string dname) {
	this->init();
	this->loadFont(dname);
}

textGenerator::~textGenerator() {
	for(int i = 0; i < 256; i++) {
		if(font[i] != nullptr) {delete font[i];}
	}
}

void textGenerator::init() {
	for(int i = 0; i < 256; i++) {
		font[i] = nullptr;
		l_leftX[i] = 0;
		l_rightX[i] = 0;
	}
	hspacing = 0;
	spaceWidth = 0.5;
}

bool textGenerator::loadFont(const boost::filesystem::path fdir) {
	typedef boost::filesystem::directory_iterator ditr;

	if (!boost::filesystem::exists(fdir)) {
		std::cout << "Font directory does not exist." << std::endl;
		return false;
	}
	
	ditr end_it;
	for(ditr it(fdir); it != end_it; ++it) {
		if(boost::filesystem::is_directory(it->status())) {continue;}
		std::string path = it->path().string();
		uint8_t cc = (it->path()).stem().string()[0];
		svfFile* sv = new svfFile(path);
		
		font[cc] = sv;
		
		//Find the letter width and xorigin
		double minX = std::numeric_limits<double>::max();
		double maxX = std::numeric_limits<double>::min();
		
		for(std::vector<std::vector<oscMusic::point>>::iterator it = (sv->data).begin(); it != (sv->data).end(); ++it) {
			for(std::vector<oscMusic::point>::iterator jt = it->begin(); jt != it->end(); ++jt) {
				if(jt->x > maxX) {maxX = jt->x;}
				if(jt->x < minX) {minX = jt->x;}
			}
		}
		
		l_leftX[cc] = minX;
		l_rightX[cc] = maxX;
	}
	
	return true;
}

void textGenerator::drawText(double freq, std::string text, textGenerator::hOrigin origin, oscMusic::point pos, double scale, double angle, double completion) {
	if(completion == 0) {return;}
	uint16_t nLetters = 0; //Can't use text.length() because not all symbols may exist in the font
	
	//Calculate the full width of the text for proper alignment
	double fullWidth = 0;
	for(std::string::iterator it = text.begin(); it != text.end(); ++it) {
		uint8_t cc = *it;
		if(*it == ' ') {fullWidth += spaceWidth; continue;}
		else if(!font[cc]) {continue;}
		fullWidth += (l_rightX[cc] - l_leftX[cc]) + hspacing;
		nLetters++;
	}
	//hspacing after the last symbol
	fullWidth -= hspacing;
	
	oscMusic::point cp;
	
	switch(origin) {
		case textGenerator::hOrigin::left: break;
		case textGenerator::hOrigin::right: cp.x-=fullWidth; break;
		case textGenerator::hOrigin::center: cp.x-=fullWidth/2.0; break;
		default: assert(false);
	}
	
	for(std::string::iterator it = text.begin(); it != text.end(); ++it) {
		uint8_t cc = *it;
		if(*it == ' ') {cp.x += spaceWidth; continue;}
		else if(!font[cc]) {continue;}
		cp.x -= l_leftX[cc];
		oscMusic::genNPolyLine(freq, cp.transformed(angle, scale, pos), angle, scale, completion, font[cc]->data);
		cp.x += (l_rightX[cc] + hspacing);
	}
}