#include <iostream>
#include <math.h>
#include "AudioFile.h"
#include <string>
#include "sceneSettings.h"
#include "simpleGenerator.h"
#include "svf.h"
#include "text.h"

using namespace std;
using namespace oscMusic;

#define SAMPLE_RATE (96000.0)

AudioFile<double> af;
AudioFile<double>::AudioBuffer buf;

class afBuffer : public oscMusic::genericBuffer {
	public:
		AudioFile<double>::AudioBuffer* buf;
		afBuffer(AudioFile<double>::AudioBuffer &b) {buf = &b;}

		void putSample(double left, double right) {
			(*buf)[0].push_back(left);
			(*buf)[1].push_back(right);
		}
};

int main() {
	svfFile cover("cover.txt");
	textGenerator tg("./font/");
	tg.hspacing = 0.2;

	
	af.setBitDepth(32);
	af.setSampleRate(SAMPLE_RATE);
	buf.resize(2);
	
	oscMusic::sceneSettings::sampleRate = SAMPLE_RATE;
	oscMusic::sceneSettings::buffer = new afBuffer(buf);
	

	cout << "Audio buffers ready" << endl;

	

	vector<oscMusic::point> pt;
	pt.push_back(oscMusic::point(0, 0));
	pt.push_back(oscMusic::point(0.5, 0.5));
	pt.push_back(oscMusic::point(0.1, 0));
	pt.push_back(oscMusic::point(0.5, -0.5));
	pt.push_back(oscMusic::point(0, 0));
	
	double nsides = 3;
	
	cout << cover.data.size() << endl;
	
	std::vector<std::vector<std::vector<oscMusic::point>>> cd;
	
	uint8_t step = 4;
	
	for(uint16_t i = 0; i < cover.data.size(); i+=step) {
		cout << (i/4) << endl;
		cd.push_back(std::vector<std::vector<oscMusic::point>>());
		for(uint16_t j = 0; j < step; j++) {
			cd[i/4].push_back(cover.data[i+j]);
		}
	}
	
	
	for(int i = 0; i <= 410; i++) {
		//Abyss
		/*
		double freq = oscMusic::fc("A4");
		tg.drawText(freq*(0.25+0.75*(min(double(i), 20.0)/20.0)), "abyss", textGenerator::hOrigin::center, oscMusic::point(0, 0), 0.3, 0, min(double(i)/70.0, 1.0));
		tg.hspacing = 0.3 + 0.02 * sin(i/5.0);
		*/
		
		//Welcome
		//double freq = oscMusic::fc("D4");
		//tg.drawText(freq, "welcome", textGenerator::hOrigin::center, oscMusic::point(0, 0), 0.2, 45, 1);
		
		//To the
		//double freq = oscMusic::fc("D3");
		//tg.drawText(freq, "to the", textGenerator::hOrigin::center, oscMusic::point(0, 0), 0.2, -45, 1);
		
		//Cover
		double par = max(min(1., min(-0.2+double(i)/180.0, 2.2-double(i)/180.0)),0.);
		//if(i % 10 == 0) {cout << par << endl;}
		
		for(uint16_t j = 0; j < cd.size(); j++) {
			double st = double(j) / double(cd.size()+1);
			if(par < st) {continue;}
			
			double modifiedPar = (par - st) / (1.0 - st);
			
			//genNPolyLine(40, oscMusic::point(0,0), 0, 1, modifiedPar, cd[j]);
		}
		
		genNPolyLine(55, oscMusic::point(0,0), 0, 1, par, cd[0]);
		genNPolyLine(55, oscMusic::point(0,0), 0, 1, par, cd[1]);
	}
	
	cout << "Buffers ready" << endl;
	
	af.setAudioBuffer(buf);
	af.save("cover.wav");
	
	return 0;
}