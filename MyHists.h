#pragma once

#include <TH1F.h>

class MyHists{
public:
  MyHists();
  virtual ~MyHists();

  //std::unique_ptr<TH1F> h_Mmumu, h_NMuon;
  TH1F /**h_Mmumu,*/ *h_NMuon;
  //std::vector<std::unique_ptr<TH1F>> get_histvec();
  std::vector<TH1F*> get_histvec();

};
