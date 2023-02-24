#include "simpleGenerator.h"
#include <algorithm>
#include <iostream>
#define OSCMUSIC_BUF (oscMusic::sceneSettings::buffer)

//Frequency modifiers
double oscMusic::f(int8_t note, int8_t oct) {return 440.0*pow(2, (note-9)/12.0 + oct - 4);}
double oscMusic::fc(std::string s) {
	uint8_t oct = 4;
	assert(s.length() != 0);
	for (auto & c: s) c = toupper(c);
	int8_t note = s[0] - 'A';
	assert(note >= 0 && note < 7);
	note = (note - 2) % 7;
	note = (note < 0 ? note+7 : note) * 2;
	note += note >= 6 ? -1 : 0;
	if(s.length() ==1) {/* yay */}
	else if (s[1] == '#') {
		note +=1;
		if(s.length() != 2) {
			oct = std::stoi(s.substr(2));
		}
	} else {oct = std::stoi(s.substr(1));}
	
	return f(note, oct);
}

void oscMusic::genCircle(double freq, double rad, oscMusic::point pos, double rot) {
	uint32_t spc = float(oscMusic::sceneSettings::sampleRate) / freq;
	for (int i = 0; i < spc; i++) {
		OSCMUSIC_BUF->putSample(
			pos.x+rad*cos(2. * M_PI * (float(i) / float(spc) + rot / 360.0)),
			pos.y+rad*sin(2. * M_PI * (float(i) / float(spc) + rot / 360.0)));
	}
}

void oscMusic::genLine(double freq,double x1, double y1, double x2, double y2) {
	uint32_t spc = float(oscMusic::sceneSettings::sampleRate) / freq;
	if(spc < 2) {spc = 2;}
	for (int i = 0; i < spc; i++) {
		OSCMUSIC_BUF->putSample(
			x1+(float(i) / float(spc))*(x2-x1),
			y1+(float(i) / float(spc))*(y2-y1));
	}
}

void oscMusic::genNPoly(double freq, double x, double y, double r, double n, double rot) {
	double step = 360 / (n);
	for (int i = 0; i < n; i++) {
		oscMusic::genLine(freq*n,
			x + lx(r, rot + i*step),
			y + ly(r, rot + i*step),
			x + lx(r, rot + (i+1)*step),
			y + ly(r, rot + (i+1)*step)
			);
	}
}

void oscMusic::genPolyLine(double freq, oscMusic::point pos, double angle, double scale, double completion, const std::vector<oscMusic::point> &p) {
	if(completion == 0) {return;}
	if(p.size() == 1) {
		point cp = p.front();
		cp *= scale;
		cp.rot(angle);
		cp += pos;
		OSCMUSIC_BUF->putSample(cp.x, cp.y);
		return;
	}
	uint32_t spc = float(oscMusic::sceneSettings::sampleRate) / (freq*completion);
	//std::cout << spc << " ^ " << p.size() << std::endl;
	assert(p.size() <= spc && p.size() < 500 && p.size() > 1);
	//Find all the &p length
	double fullLength = 0;
	double l[500];
	uint16_t lc[500];
	for(int i = 0; i < p.size()-1; i++) {
		double len = (p[i+1]-p[i]).abs();
		//assert(len!=0);
		l[i] = fullLength;
		fullLength += len;
	}
	l[p.size()-1] = fullLength;
	double step = fullLength / double(spc-1);

	uint16_t curSPC = 0;
	
	for(int i = 0; i < p.size(); i++) {l[i] = round(l[i] / step);}
	for(int i = 0; i < p.size()-1; i++) {
		if(curSPC > completion * double(spc)) {break;}
		for(int j = 0; j < (l[i+1]-l[i]); j++) {
			point cp = p[i]+(p[i+1]-p[i]).norm() * step * j;
			cp *= scale;
			cp.rot(angle);
			cp += pos;
			OSCMUSIC_BUF->putSample(cp.x, cp.y);
			curSPC++;
			if(curSPC > completion * double(spc)) {break;}
		}
	}
	
	point cp = p.back();
	cp *= scale;
	cp.rot(angle);
	cp += pos;
	OSCMUSIC_BUF->putSample(cp.x, cp.y);
}

void oscMusic::genNPolyLine(double freq, point pos, double angle, double scale, double completion, const std::vector<std::vector<oscMusic::point>> &p) {
	//Calculate the length of each curve
	double flen = 0;
	double len[500];
	for(uint16_t i = 0; i < p.size(); i++) {
		len[i] = 0;
		for(auto it = p[i].begin(); it != p[i].end()-1; it++) {
			len[i] += (*it - *(it+1)).abs();
		}
		flen += len[i];
	}
	for(uint16_t i = 0; i < p.size(); i++) {
		double tf = freq;
		if(len[i] != 0) {
			tf = freq * flen / len[i];
		}
		oscMusic::genPolyLine(tf, pos, angle, scale, completion, p[i]);
	}
}