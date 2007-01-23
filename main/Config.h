/** Config
 *
 * Main program configuration.
 *
 * Parse command line args in constructor, will abort with a message
 * on failure.
 *
 */

#ifndef CONFIG_H
#define CONFIG_H

#include "OscParam.h"
#include <string>

class Options;

struct Config {

    OscParam op;		// Oscillation parameters
    int nu_num;			// Initial nu
    
    std::string energy_desc;	// energy range description
    std::string baseline_desc;	// baseline range description
    std::string calc_desc;	// calculation type description
    std::string density_desc;	// density description

    /// Create a main program configuration
    Config(int argc, const char** argv);
    ~Config();

    /// Spew usage and exit
    void usage(const char* message, bool full=false);

    /// Return a human readable string describing setup
    std::string as_string() const;

private:
    Options* options;

};

#endif  // CONFIG_H
