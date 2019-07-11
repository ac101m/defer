// Standard
#include <iostream>

// External
#include <GLT/Window.hpp>
#include <optparse.hpp>


// Add command line optioons to the option parser
void AddOptions(OptionParser& opt) {
  opt.Add(Option("displayx", 'x', ARG_TYPE_INT,
                 "Set width of the render area",
                 {"1024"}));
  opt.Add(Option("displayy", 'y', ARG_TYPE_INT,
                 "Set height of the render area",
                 {"768"}));
}


int main(int argc, char **argv) {
  OptionParser opt(argc, argv, "Simple immediate renderer");
  AddOptions(opt);

  // Get display parameters and open a window
  int displayx = opt.Get("displayx");
  int displayy = opt.Get("displayy");
  GLT::Window window = GLT::Window(displayx, displayy, "immediate");

  // Main render loop
  while(!window.ShouldClose()) {
    window.Refresh();
  }

  return 0;
}
