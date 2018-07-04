#include <math.h>

#include "TH1F.h"
#include "TH2D.h"
#include <iostream>
#include "MyHists.h" 
#include <TError.h> 

using namespace std;

MyHists::MyHists(){

  // Set up all the histograms


   // Try to change the bin ranges such that the spectrum peaks at the mass of the Z boson.
  /*
  h_Mmumu = new TH1F("Mmumu", "Invariant di-muon mass", 90, 0, 90);
  h_Mmumu->SetXTitle("M_{#mu#mu} [GeV]");
  h_Mmumu->Sumw2();
  */
  h_NMuon = new TH1F("NMuon", "Number of muons", 7, 0, 7);
  h_NMuon->SetXTitle("No. Muons");
  h_NMuon->Sumw2();


}

MyHists::~MyHists(){
  delete h_NMuon;
}

//std::vector<std::unique_ptr<TH1F>> MyHists::get_histvec(){
std::vector<TH1F*> MyHists::get_histvec(){
  gErrorIgnoreLevel = 2002;
  std::vector<TH1F*> vec;
  //vec.emplace_back(h_Mmumu);
  TH1F* NMuon = (TH1F*)h_NMuon->Clone();
  vec.emplace_back(NMuon);

  return vec;
}
