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

   vector<pair<TString, TString>> processes (9);
   
   processes[0] = make_pair("Data", "data");
   processes[1] = make_pair("TTbar", "ttbar");
   processes[2] = make_pair("Wjets", "wjets");
   processes[3] = make_pair("DY", "dy");
   processes[4] = make_pair("WW", "ww");
   processes[5] = make_pair("WZ", "wz");
   processes[6] = make_pair("ZZ", "zz");
   processes[7] = make_pair("QCD", "qcd");
   processes[8] = make_pair("single Top", "single_top");


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
   for(int i = 0; i < processes.size(); i++){
   
     MyAnalysis* A = new MyAnalysis();
     std::unique_ptr<TChain> ch;
     ch.reset(new TChain("events"));
     TString filename = "files/" + processes[i].second + ".root";
     ch->Add(filename);
     cout << endl << "Now starting to process the sample '" << processes[i].first << "'" << endl; 
     ch->Process(A);

     //Here you can access variables, defined in MyAnalysis.C 
     // float 'new var' = A -> 'variable';


     // Add this process to the plotter
     if(processes[i].first == "Data"){
       int idx = 0;
       for(std::map<TString,MyHists*>::iterator it2=A->GetHistmap()->begin(); it2!=A->GetHistmap()->end(); ++it2){
	 Plotters[idx].SetData(A->GetHists(it2->first)->get_histvec(), string(processes[i].first));
	 idx++;
       }
     }
     else{
       int idx = 0;
       for(std::map<TString,MyHists*>::iterator it2=A->GetHistmap()->begin(); it2!=A->GetHistmap()->end(); ++it2){
	 Plotters[idx].AddBg(A->GetHists(it2->first)->get_histvec(), std::string(processes[i].first));
	 Plotters_MC[idx].AddBg(A->GetHists(it2->first)->get_histvec(), std::string(processes[i].first));
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
