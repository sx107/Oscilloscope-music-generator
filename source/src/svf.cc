#include "svf.h"
#include <fstream>
#include <iostream>

using namespace std;

svfFile::svfFile() {};
svfFile::svfFile(std::string fname) {this->open(fname);}
bool svfFile::open(std::string fname) {
	std::ifstream fs;
	fs.open(fname);
	if(!fs) {return false;}
	std::string line;
	fs >> line;
	if(line != "SVF") {cout << "SVF header not present." << endl; return false;}
	
	//Now read all the curves
	while(!fs.eof()) {
		fs >> line;
		if(fs.eof() || line == " " || line == "") {break;}
		if(line != "CURVE") {cout << "Expected CURVE, got instead: \"" << line << "\"" << endl; return false;}
		uint16_t curveId, numPoints;
		fs >> curveId >> numPoints;
		if(numPoints == 0) {continue;}
		data.push_back(vector<oscMusic::point>());
		for(int i = 0; i < numPoints; i++) {
			double px, py;
			fs >> px >> py;
			data.back().push_back(oscMusic::point(px, py));
		}
	}
	return true;
}