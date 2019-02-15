// Filename:  decayExample.C
// Author:    Marjorie Shapiro
// Date       Sept 12,2009
//
// This macro is an example showing how to write a simple
// Monte Carlo using ROOT.
//
// To run this macro startup ROOT and then do:
//
// .L decayExample.C++
// doDecays(10000,10.0,20.0);
//
// To exit ROOT type
// .q
//
// The output of this macro is a gif file called decayPlots.gif and
// a root file called decayExample.root
//
// Here is the problem we are solving:
// 
// A beam of 10,000 B+ particles with lifetime of 1.638e-12 and mass
// 5.279 GeV start at x=0 and are travel in the +x-direction with momenta 
// uniformly distributed between 10 and 20 GeV.
//  a) Make a 1 dimensional histogram of the number of particles that decay 
//     a function of x for x between 0 and 1 cm 
//  b) Make a profile plot of the mean decay distance as a function of 
//     B+ momentum
//     (a profile plot gives the average value of one variable as a function
//      of another)
//
// Note:  Comments  in ++ start with two backslashes
// You need to include at the top of your file the header files
// defining any classes you use.
# 
#include <iostream>      // This is the C++ class for print things
                         // All Root classes have names starting with T
#include "TRandom.h"     // This is the random number class in Root
#include "TH1F.h"        // A profile plot gives the mean and variance per bin
#include "TProfile.h"
#include "TCanvas.h"
#include "TFile.h"

using std::cout;
using std::endl;

// This declares our class
// The input is the number of events, the min and max momentum 
// You need to declare for each variable if it is an integer, float or double
// the function type void means that EMShower doesnt' return a value 
//
// General rules:
//   All statements end with a ;
//   All variables and functions must be declared as integer, float or double
//   You can initialize variables when you declare them. 
//
// We've set this up so that the number of events, Pmin and Pmax are passed
// to the code but the lifetime is set in the code
//
void doDecays(int numEvt, double Pmin, double Pmax) {

  double tau = 1.638e-12; //seconds
  double mass = 5.279; // GeV
  double c = 3.0e10;  // cm per sec

  // Create one histogram and one Profile plot
  // Arguments are: Name of Profile, Title, number of bins,min, max
  TProfile hProf("hProf","Mean Decay distance vs momentum",100,Pmin,Pmax);

  // Arguments are: Name of histogram, Title, number of bins,min, max
  TH1F hist("hist","Number of Decays vs distance",100,0,1.0);

  // We need to create an object that is a random number generator
  TRandom* random = new TRandom();

  cout << "About to generate " << numEvt << " events" 
       << " with Pmin=" << Pmin << " and Pmax = " << Pmax << endl;
  for (int i=0; i<numEvt; i++) {
    // Throw dice to get the momentum of the particle
    // TRandom::Uniform(x) throws a flat distribution from 0 to x
    double momentum = Pmin+random->Uniform(Pmax-Pmin);
    // Now let's find the proper decay length
    double energy = sqrt(momentum*momentum+mass*mass);
    double beta = momentum/energy;
    double gamma = 1.0/sqrt(1.0-beta*beta);
    // Throw dice to get the decay length
    // TRandom::Exp(tau) generates according to Exp(-t/tau)
    double distance=random->Exp(gamma*c*tau);
    //        
    // BTW, if you created something with a "new" you use a "->" to get to
    // its methods.  If you created it with just a declaration you use a "."
    hist.Fill(distance);
    hProf.Fill(momentum,distance);
  }
  // Make a canvase and put the two plots on it.  Then write it to a file
  TCanvas c1;
  c1.Divide(1,2);
  c1.cd(1);
  hist.Draw();
  c1.cd(2);
  hProf.Draw();
  c1.Print("decayPlots.gif");
  // Create a root file and write the histogram and profile to it in
  // root format.  You can then display them later in root
  // WARNING: If you run this macro twice, you will overwrite the root file
  // If you change "recreate" to "new" then the macro will signal an error
  // and fail to write a file if a file with the specified name already exists
  TFile f("decayExample.root","recreate");
  hProf.Write();
  hist.Write();

}           //End of macro
