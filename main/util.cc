#include "util.h"
#include <nuosc_earth.h>
#include <cmath>
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
    A single distance value:    "one:value"
    Zenith angle:               "zen:start,stop,step"
    cos(Zenith angle):          "cos:start,stop,step"
*/

vector<double> desc2vector(string desc)
{
    string type = desc.substr(0,3);
    vector<double> data = strings2doubles(split(desc.substr(4),','));

    vector<double> vd;

    if (type == "one") {
	vd.push_back(data[0]);
	return vd;
    }

    if (type == "lin") {
	for (double value=data[0]; value <= data[1]; value += data[2]) 
	    vd.push_back(value);
	return vd;
    }

    if (type == "log") {
	for (double value=data[0]; value <= data[1]; value += data[2]) 
	    vd.push_back(pow(10,value));
	return vd;
    }
    
    if (type == "zen") {
	double depth = 0.0;
	if (data.size() == 4) depth = data[3];
	for (double value=data[0]; value <= data[1]; value += data[2])
	    vd.push_back(earth_zenith_to_baseline(value, depth));
	return vd;
    }

    if (type == "cos") {
	double depth = 0.0;
	if (data.size() == 4) depth = data[3];
	double start = data[0], stop = data[1];
	if (start > stop) start = data[1], stop = data[0];
	for (double value=start; value <= stop; value += data[2])
	    vd.push_back(earth_zenith_to_baseline(acos(value), depth));
	return vd;
    }

    return vd;			// empty
}
