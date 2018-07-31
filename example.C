#include "MyAnalysis.h"
#include "Plotter.h"
#include <iostream>
#include <TChain.h>
#include <TString.h>
#include <TGraphAsymmErrors.h>
#include <string>

int main() {
   
   // Integrated luminosity (this value is needed for the calculation of the cross section)
   float lumi = 50.;

   // This map stores all samples to run over
   std::map<TString, TString> processes;
   processes["Data"] = "data";
   processes["Wjets"] = "wjets";
   processes["DY"] = "dy";
   processes["TTbar"] = "ttbar";
   processes["WW"] = "ww";
   processes["WZ"] = "wz";
   processes["ZZ"] = "zz";
   processes["QCD"] = "qcd";
    processes["single Top"] = "single_top";

   // Set up plotters
   std::vector<Plotter> Plotters, Plotters_MC;  
   MyAnalysis* dummy = new MyAnalysis();
   dummy->BuildHistmap();
   for(std::map<TString,MyHists*>::iterator it=dummy->GetHistmap()->begin(); it!=dummy->GetHistmap()->end(); ++it){
     std::cout << "Setting up the following set of histograms: " << it->first <<  endl;
     TString outname = it->first;
     Plotter p, p_mc;
     p.SetOutname(outname);
     p_mc.SetOutname(outname+"_MC");
     Plotters.emplace_back(p);
     Plotters_MC.emplace_back(p_mc);
   }
   delete dummy;

   
   // Iterate over elements of map to chain all processes
   for(std::map<TString,TString>::iterator it=processes.begin(); it!=processes.end(); ++it){
   
     MyAnalysis* A = new MyAnalysis();
     std::unique_ptr<TChain> ch;
     ch.reset(new TChain("events"));
     TString filename = "files/" + it->second + ".root";
     ch->Add(filename);
     cout << endl << "Now starting to process the sample '" << it->first << "'" << endl; 
     ch->Process(A);

     // Add this process to the plotter
     if(it->first == "Data"){
       int idx = 0;
       for(std::map<TString,MyHists*>::iterator it2=A->GetHistmap()->begin(); it2!=A->GetHistmap()->end(); ++it2){
	 Plotters[idx].SetData(A->GetHists(it2->first)->get_histvec(), string(it->first));
	 idx++;
       }
     }
     else{
       int idx = 0;
       for(std::map<TString,MyHists*>::iterator it2=A->GetHistmap()->begin(); it2!=A->GetHistmap()->end(); ++it2){
	 Plotters[idx].AddBg(A->GetHists(it2->first)->get_histvec(), std::string(it->first));
	 Plotters_MC[idx].AddBg(A->GetHists(it2->first)->get_histvec(), std::string(it->first));
	 idx++;
       }
     }
     delete A;
   }

   // Plot everything
   for(unsigned int i=0; i<Plotters.size(); i++){
     Plotters[i].Plot();
     Plotters_MC[i].Plot();
   }
   cout << endl << endl << "Done processing and plotting!" << endl;





   ///////////////////////////////////////
   //                                   //
   //      Add your new code below      // ++++++++++++++++++++++++++++++++++++++++++++++++++++++
   //                                   //
   ///////////////////////////////////////





















   //////////////////////////////
   // Exercise 3: Measurement of the cross section
   // Get the efficiency of your event selection on ttbar. Calculate the number of (weighted) selected events divided
   // by the number of (weighted) generated events.
   // double eff = ...;

   // Subtract the non-ttbar background from data
   // ...

   // Calculate the cross section
   // ...

   // Propagate an uncertainty on the cross section
   // ...


}
