#ifndef DEFER_OPTIONS_INCLUDED
#define DEFER_OPTIONS_INCLUDED


// External
#include <optparse.hpp>


// Add command line optioons to the option parser
void BuildOptionParser(OptionParser& opt) {
  opt.Add(Option("displayx", 'x', ARG_TYPE_INT,
                 "Set width of the render area",
                 {"1024"}));
  opt.Add(Option("displayy", 'y', ARG_TYPE_INT,
                 "Set height of the render area",
                 {"768"}));
  opt.Add(Option("lights", 'l', ARG_TYPE_INT,
                 "Set number of lights",
                 {"64"}));
  opt.Add(Option("fullscreen", 'f', ARG_TYPE_VOID,
                 "Run in fullscreen if defined"
                 ));
}


#endif // DEFER_OPTIONS_INCLUDED
