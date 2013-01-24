#include "util.h"
#include <nuosc_earth.h>
#include <cmath>
#include <cstdlib>
#include <iostream>
using namespace std;
vector<string> split(string s, char c)
{
    vector<string> vs;
    while (true) {
	size_t comma = s.find(c);
	vs.push_back(s.substr(0,comma));
	if (comma == string::npos) break;
	s = s.substr(comma+1);
    }
    return vs;
}
vector<double> strings2doubles(const vector<string>& vs)
{
    vector<double> vd;
    for (size_t ind=0; ind<vs.size(); ++ind) {
	vd.push_back(atof(vs[ind].c_str()));
    }
    return vd;
}

/** Convert a description string into a vector of values
    Linear disance range:       "lin:start,stop,step"
    Logarithmic distance range: "log:start,stop,step"
    A single distance value:    "one:value" or just "value"
    Zenith angle:               "zen:start,stop,step"
    cos(Zenith angle):          "cos:start,stop,step"
*/

vector<double> desc2vector(string desc)
{
    vector<double> dir;
    return desc2vector(desc,dir);
}
vector<double> desc2vector(string desc, vector<double>& dir)
{
    string type = "one";
    size_t start = 0;
    if (desc[3] == ':') {
        type = desc.substr(0,3);
        start = 4;
    }
    vector<double> data = strings2doubles(split(desc.substr(start),','));

    vector<double> vd;

    if (type == "one") {
	vd.push_back(data[0]);
	dir.push_back(data[0]);
	return vd;
    }

    if (type == "lin") {
	double stop = data[1] + 0.001*data[2];
	for (double value=data[0]; value <= stop; value += data[2]) {
	    vd.push_back(value);
	    dir.push_back(value);
	}
	return vd;
    }

    if (type == "log") {
	double stop = data[1] + 0.001*data[2];
	for (double value=data[0]; value <= stop; value += data[2]) {
	    vd.push_back(pow(10,value));
	    dir.push_back(value);
	}
	return vd;
    }
    
    if (type == "zen") {
	double depth = 0.0;
	double stop = data[1];
	if (data.size() == 4) depth = data[3]*1.0e5;
	stop += 0.001*data[2];
	
	for (double value=data[0]; value <= stop; value += data[2]) {
	    double cm = earth_zenith_to_baseline(value*M_PI/180.0, depth);
	    vd.push_back(cm/1.0e5);
	    dir.push_back(value);
	}
	return vd;
    }

    if (type == "cos") {
	double depth = 0.0;
	if (data.size() == 4) depth = data[3]*1.0e5;
	double start = data[0], stop = data[1];
	if (start > stop) start = data[1], stop = data[0];
	stop += 0.001*data[2];

	for (double value=start; value <= stop; value += data[2]) {
	    double cm = earth_zenith_to_baseline(acos(value), depth);
	    vd.push_back(cm/1.0e5);
	    dir.push_back(value);
	    //cerr << "\t" << value << " " << cm << endl;
	}

	//cerr << "cos:" << start << "," << stop << "," << data[2] << "," << depth << endl;
	return vd;
    }

    return vd;			// empty
}
