#include "util.h"

#include <vector>
#include <string>
#include <iostream>
using namespace std;
int main (int argc, char *argv[])
{
    const char* desc[] = {
	"zen:0.0,180.0,1.8,1",
	"cos:-1,0,0.01,1",
	"log:-1,1,.02",
	0
    };

    for (int ind=0; desc[ind]; ++ind) {
	vector<double> ret,dir;
	ret = desc2vector(desc[ind],dir);
	cout << desc[ind] << endl
	     << "\tret: " << ret.size() 
	     << ", " << ret[0] << " --> " << ret.back() << endl
	     << "\tdir: " << dir.size()
	     << ", " << dir[0] << " --> " << dir.back() << endl;
    }
    return 0;
} // end of main()
