#pragma once
#include <cmath>
#include "assert.h"

namespace oscMusic {
	enum class cSystem
	{
		Polar,
		Rect
	};
	
	inline double dtr(double angle) {return M_PI * angle / 180.0;}
	
	//Lx and Ly functions
	inline double lx(double r, double phi) {return r*cos(M_PI*phi/180.0);}
	inline double ly(double r, double phi) {return r*sin(M_PI*phi/180.0);}
	
	struct point {
		double x;
		double y;
		point(double x_, double y_): x(x_), y(y_) {};
		//point(point& p) : x(p.x), y(p.y) {};
		point() : x(0), y(0) {};
		
		void set(double a, double b, cSystem cs = cSystem::Rect);
		void rot(double angle);
		point transformed(double angle = 0, double scale = 1, point translate = point());
		
		double abs();
		point& norm();
		//Operators
		point operator+=(const point &r) {x += r.x; y += r.y;}
		point operator-=(const point &r) {x -= r.x; y -= r.y;}
		point operator*=(const double c) {x*=c; y*=c;}
		point operator/=(const double c) {x/=c; y/=c;}
		friend point operator+(point l, const point &r) {l+=r; return l;}
		friend point operator-(point l, const point &r) {l-=r; return l;}
		friend point operator*(point l, const double r) {l*=r; return l;}
		friend point operator/(point l, const double r) {l/=r; return l;}
	};
}