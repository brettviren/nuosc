#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <string>

/// Split a string into a vector of strings at the given delimiter
std::vector<std::string> split(std::string s, char delimiter);

/// Convert a vector of strings to a vector of doubles
std::vector<double> strings2doubles(const std::vector<std::string>& vs);

/** Convert a description string into a vector of values
    Linear disance range:       "lin:start,stop,step"
    Logarithmic distance range: "log:start,stop,step"
    A single distance value:    "one:value"
    Zenith angle:               "zen:start,stop,step"
    cos(Zenith angle):          "cos:start,stop,step"
*/

std::vector<double> desc2vector(std::string desc);

#endif  // UTIL_H
