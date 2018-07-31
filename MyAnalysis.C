#define MyAnalysis_cxx
// The class definition in MyAnalysis.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("MyAnalysis.C")
// Root > T->Process("MyAnalysis.C","some options")
// Root > T->Process("MyAnalysis.C+")
//

#include "MyAnalysis.h"
#include <iostream>
#include <TH1F.h>
#include <TLatex.h>
#include "MyHists.h"

using namespace std;

void MyAnalysis::BuildEvent() {

  Muons.clear();
  for (int i = 0; i < NMuon; ++i) {
    MyMuon muon(Muon_Px[i], Muon_Py[i], Muon_Pz[i], Muon_E[i]);
    muon.SetIsolation(Muon_Iso[i]);
    muon.SetCharge(Muon_Charge[i]);
    Muons.push_back(muon);
  }
   
  Electrons.clear();
  for (int i = 0; i < NElectron; ++i) {
    MyElectron electron(Electron_Px[i], Electron_Py[i], Electron_Pz[i], Electron_E[i]);
    electron.SetIsolation(Electron_Iso[i]);
    electron.SetCharge(Electron_Charge[i]);
    Electrons.push_back(electron);
  }
   
  Photons.clear();
  for (int i = 0; i < NPhoton; ++i) {
    MyPhoton photon(Photon_Px[i], Photon_Py[i], Photon_Pz[i], Photon_E[i]);
    photon.SetIsolation(Photon_Iso[i]);
    Photons.push_back(photon);
  }
   
  Jets.clear();
  for (int i = 0; i < NJet; ++i) {
    MyJet jet(Jet_Px[i], Jet_Py[i], Jet_Pz[i], Jet_E[i]);
    jet.SetBTagDiscriminator(Jet_btag[i]);
    jet.SetJetID(Jet_ID[i]);
    Jets.push_back(jet);
  }
   
  hadB.SetXYZM(MChadronicBottom_px, MChadronicBottom_py, MChadronicBottom_pz, 4.8);
  lepB.SetXYZM(MCleptonicBottom_px, MCleptonicBottom_py, MCleptonicBottom_pz, 4.8);
  hadWq.SetXYZM(MChadronicWDecayQuark_px, MChadronicWDecayQuark_py, MChadronicWDecayQuark_pz, 0.0);
  hadWqb.SetXYZM(MChadronicWDecayQuarkBar_px, MChadronicWDecayQuarkBar_py, MChadronicWDecayQuarkBar_pz, 0.0);
  lepWl.SetXYZM(MClepton_px, MClepton_py, MClepton_pz, 0.0);
  lepWn.SetXYZM(MCneutrino_px, MCneutrino_py, MCneutrino_pz, 0.0);
  met.SetXYZM(MET_px, MET_py, 0., 0.);
   
  EventWeight *= weight_factor;
}

void MyAnalysis::Begin(TTree * /*tree*/) {
   // The Begin() function is called at the start of the query.
   // When running with PROOF Begin() is only called on the client.
   // The tree argument is deprecated (on PROOF 0 is passed).
   
   TString option = GetOption();
   
}

void MyAnalysis::SlaveBegin(TTree * /*tree*/) {
   // The SlaveBegin() function is called after the Begin() function.
   // When running with PROOF SlaveBegin() is called on each slave 
   // The tree argument is deprecated (on PROOF 0 is passed).
  
   TString option = GetOption();

   BuildHistmap();
}

void MyAnalysis::FillHistos(MyHists & h){

  //TODO: Add more histos!
  h.h_NMuon->Fill(NMuon, EventWeight);
  if(N_IsoMuon>0){
    h.h_MuonPt->Fill(muon1->Pt(), EventWeight);
    h.h_MuonEta->Fill(muon1->Eta(), EventWeight);
    h.h_MuonPhi->Fill(muon1->Phi(), EventWeight);
  }
 
  h.h_NJets->Fill(NJet, EventWeight);
  
  if(N_Jets >0){  
    h.h_Jet1_pt->Fill(jet1->Pt(), EventWeight);
    h.h_Jet1_Eta->Fill(jet1->Eta(), EventWeight);
    h.h_Jet1_Phi->Fill(jet1->Phi(), EventWeight);
  }
  if(N_Jets >1){
    h.h_Jet2_pt->Fill(jet2->Pt(), EventWeight);
    h.h_Jet2_Eta->Fill(jet2->Eta(), EventWeight);
    h.h_Jet2_Phi->Fill(jet2->Phi(), EventWeight);
  }

  h.h_NbJets->Fill(N_bJets, EventWeight);
  if(N_bJets >0){
     h.h_bJet1_pt->Fill(b_jet1->Pt(), EventWeight);
     h.h_bJet1_Eta->Fill(b_jet1->Eta(), EventWeight);
     h.h_bJet1_Phi->Fill(b_jet1->Phi(), EventWeight);
  }
  if(N_bJets >1){
     h.h_bJet2_pt->Fill(b_jet2->Pt(), EventWeight);
     h.h_bJet2_Eta->Fill(b_jet2->Eta(), EventWeight);
     h.h_bJet2_Phi->Fill(b_jet2->Phi(), EventWeight);
  }				      
}


void MyAnalysis::PrintModule(Long64_t entry) {

//   ++++++++++++++++++++++++++++++++++++++++
//   Print Module
//   ++++++++++++++++++++++++++++++++++++++++
     cout<< "New Event: "<<endl;
     cout << "Jets: " << endl;
     for (vector<MyJet>::iterator it = Jets.begin(); it != Jets.end(); ++it) {
     cout << "pt, eta, phi, btag, id: " << it->Pt() << ", " << it->Eta() << ", " << it->Phi() << ", " << it->IsBTagged() << ", " << it->GetJetID()
     << endl;
     }
     cout << "Muons: " << endl;
     for (vector<MyMuon>::iterator it = Muons.begin(); it != Muons.end(); ++it) {
     cout << "pt, eta, phi, iso, charge: " << it->Pt() << ", " << it->Eta() << ", " << it->Phi() << ", "
     << it->GetIsolation() << ", " << it->GetCharge() << endl;
     }
     cout<<""<<endl;
}



Bool_t MyAnalysis::Process(Long64_t entry) {
   // The Process() function is called for each entry in the tree (or possibly
   // keyed object in the case of PROOF) to be processed. The entry argument
   // specifies which entry in the currently loaded tree is to be processed.
   // It can be passed to either MyAnalysis::GetEntry() or TBranch::GetEntry()
   // to read either all or the required parts of the data. When processing
   // keyed objects with PROOF, the object is already loaded and is available
   // via the fObject pointer.
   //
   // This function should contain the "body" of the analysis. It can contain
   // simple or elaborate selection criteria, run algorithms on the data
   // of the event and typically fill histograms.
   //
   // The processing can be stopped by calling Abort().
   //
   // Use fStatus to set the return value of TTree::Process().
   //
   // The return value is currently not used.

   ++TotalEvents;
   
   GetEntry(entry);
   
   if (TotalEvents % 10000 == 0)
     cout << "Next event -----> " << TotalEvents << endl;
   
   BuildEvent();

   double MuonRelIsoCut = 0.10;
   N_IsoMuon = 0;
   N_Jets = 0;
   N_bJets = 0;
   
   // Loop over all muons. The number of isolated muons 'N_IsoMuon' is summed up. 
   // The leading muon and the second leading muon get defined by pointing to the 
   // respective iterator (muon object).
   for (vector<MyMuon>::iterator jt = Muons.begin(); jt != Muons.end(); ++jt) {
      if (jt->IsIsolated(MuonRelIsoCut)) {
         ++N_IsoMuon;
         if (N_IsoMuon == 1) muon1 = &(*jt);
         if (N_IsoMuon == 2) muon2 = &(*jt);
      }
   }

   //Loop over all jets. 
   for(vector<MyJet>::iterator it = Jets.begin(); it != Jets.end(); ++it){
      ++N_Jets;
     
      if (N_Jets == 1) jet1 = &(*it);
      if (N_Jets == 2) jet2 = &(*it);
          
      //Identify b-tagged jets
      if(it->IsBTagged()){
	++N_bJets;
	if (N_bJets == 1) b_jet1 = &(*it);
	if (N_bJets == 2) b_jet2 = &(*it);
      }
   }
   
   //cout << "Going to fill hists" << endl;
   FillHistos(hists_nocuts);
   //cout << "Filled Hists" << endl;

    
   // +++++++++++++++++++++++++++++++++++++++++++
   // Exercise 1: MC / data comparisons
   // +++++++++++++++++++++++++++++++++++++++++++
   
   //Define Cut values
   double MuonPtCut = 25.;

   //Do not Change!! Require at least one isolated muon
   if (N_IsoMuon > 0 && triggerIsoMu24) {
     
     // Have a look at your histograms and compare the different background samples.
     // Try to enrich the fraction of ttbar by cutting on
     // any of the distributions 

     //Plot all variables after every cut; define new Set of Hists in MyAnalysis.h 
     if (muon1->Pt()>MuonPtCut) {
       FillHistos(hists_firstcuts);
     }
   }

   // Once you have optimized your event selection, add an variable to find the
   // fraction that survives your event selection. 




   // +++++++++++++++++++++++++++++++++++++++++++
   // Exercise 2: Measurement of the cross section
   // +++++++++++++++++++++++++++++++++++++++++++
   // Define a float (in Analysis.h) for the weighted number of selected events. Also, 
   // define a float for the weighted number of generated events. With these numbers, 
   // the selection efficiency can be calculated. Afterwards, move to the 'example.C' 
   // file to determine the efficiency and the ttbar cross section.


   //cout << "going to return" << endl;
   return kTRUE;
}




void MyAnalysis::SlaveTerminate() {
   // The SlaveTerminate() function is called after all entries or objects
   // have been processed. When running with PROOF SlaveTerminate() is called
   // on each slave server.
   
}

void MyAnalysis::Terminate() {
   // The Terminate() function is the last function to be called during
   // a query. It always runs on the client, it can be used to present
   // the results graphically or save the results to file.
   
}
