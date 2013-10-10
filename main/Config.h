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
    std::string mix_desc; 	// mixing angle description

    std::string data_file;        // output data file (def=stdout)
    std::string info_file;        // output meta data file (def=stderr)
    std::string cmdline_file;	  // output meta data file (def=)

    /// Create a main program configuration
    Config(int argc, const char** argv);
    ~Config();

    /// Spew usage and exit
    void usage(const char* message, bool full=false);

    /// Return a human readable string describing setup
    std::string as_string() const;

    /// As above but exactly matching command line args
    std::string as_string_cmdline() const;

private:
    Options* options;

};

#endif  // CONFIG_H
