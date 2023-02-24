#pragma once
#include <string>
#include <math.h>
#include <assert.h>
#include "sceneSettings.h"
#include "genericBuffer.h"
#include <vector>
#include "point.h"

namespace oscMusic {			
	//Frequency functions
	double f(int8_t note, int8_t oct = 4);
	double fc(std::string c);
	
	void genCircle(double freq, double rad, point pos, double rot = 0);
	void genLine(double freq, double x1, double y1, double x2, double y2);
	void genNPoly(double freq, double x, double y, double r, double n, double rot = 0);
	void genPolyLine(double freq, point pos, double angle, double scale, double completion, const std::vector<oscMusic::point> &p);
	void genNPolyLine(double freq, point pos, double angle, double scale, double completion, const std::vector<std::vector<oscMusic::point>> &p);
}