#include "point.h"

void oscMusic::point::set(double a, double b, oscMusic::cSystem cs) {
	switch(cs) {
		case oscMusic::cSystem::Rect:
			x = a; y = b;
			break;
		case oscMusic::cSystem::Polar:
			x = a*cos(dtr(b)); y = a*sin(dtr(b));
			break;
		default:
			assert(false);
	}
}

void oscMusic::point::rot(double angle) {
	double tx = x*cos(oscMusic::dtr(angle)) - y*sin(oscMusic::dtr(angle));
	double ty = x*sin(oscMusic::dtr(angle)) + y*cos(oscMusic::dtr(angle));
	x = tx;
	y = ty;
}

oscMusic::point oscMusic::point::transformed(double angle, double scale, oscMusic::point translate) {
	oscMusic::point p = *this;
	p.rot(angle);
	p*=scale;
	p += translate;
	return p;
}

double oscMusic::point::abs() {
	return sqrt(x*x+y*y);
}

oscMusic::point& oscMusic::point::norm() {
	(*this)/=(this->abs());
	return *this;
}