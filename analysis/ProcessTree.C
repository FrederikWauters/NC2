
#include "TRandom2.h"
#include "TMath.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include "TCanvas.h"
#include "TStyle.h"
#include "TTree.h"
#include "TROOT.h"
#include <time.h>
#include "TStopwatch.h"
#include <algorithm>
#include <map>
#include <stdlib.h>



//Global variables

static double resolution=3; //FWHM, in keV

static double bgoNPhotElectrons_keV = 8200*0.1*0.001; //Literature light yield * photo-electron efficiency * MeV->keV  

TFile* infile;
TFile* outfile;
TTree* tree;
TTree* runTree;

TH2F* hEdep;
TH2F* hEdep2s;
TH2F* hEdep2s1s;
TH1D* hEInit;

TH2F* hE3p2s;
TH2F* hE4p2s;
TH2F* hE5p2s;

TH2F* hE3p2sHas2s;
TH2F* hE4p2sHas2s;
TH2F* hE5p2sHas2s;

//Which spectra to look at?
//
// E with all
// E with 2s passage
// E with 2s-1s transition
//
// E with 3p2s coincidence versus coinc pair
// E with 4p2s coincidence versus coinc pair
// E with 5p2s coincidence versus coinc pair

// E with 3p2s coincidence versus coinc pair with 2s passage
// E with 4p2s coincidence versus coinc pair with 2s passage
// E with 5p2s coincidence versus coinc pair with 2s passage

static const int NDET = 10;
std::string detectors[NDET] = {"Ge1","Ge2","Ge3","Ge4","Ge5","Ge6","Ge7","Ge8","Ge9","Ge10"};

std::map<std::string,double> create_map()
{
  std::map<std::string,double> m;
  m["3p2s"] = 316.5;
  m["4p2s"] = 440.3;
  m["5p2s"] = 494.5;
  return m;
}
std::map<std::string,double> transition_energies = create_map();


static bool DEBUG = false;
//static bool DEBUG = true;

static unsigned long int NInitEvents;

void LoopTree(int maxEvents = -1)
{
  if(maxEvents < 0 || maxEvents > tree->GetEntries()) maxEvents = tree->GetEntries();
  if(maxEvents < 1) return;
  
  TRandom2* random_gen = new TRandom2();
  
  //set variables  

  std::map<std::string, double>* energyDepositions = new std::map<std::string, double>();
  std::vector<double>* initEs = new std::vector<double>(); //set from primary generator
  std::vector<std::string>* levels = new std::vector<std::string>();
      
  tree->SetBranchAddress("Edeps",&energyDepositions);
  tree->SetBranchAddress("EInits",&initEs);
  tree->SetBranchAddress("Levels",&levels);
  
  //record time
  TStopwatch timer;
  timer.Start();
  std::cout << "Start processing " << maxEvents << std::endl;
  
  
  //loop
  for(int i =0; i<maxEvents; i++)
  {
    tree->GetEntry(i);
    if(i%1000000 == 0) { std::cout << "entry " << i << std::endl; }
    
    if(DEBUG && i < 100)
    {
      std::cout << " entry = " << i << std::endl;
      std::cout << " size initEs : " << initEs->size() << std::endl;
        for(unsigned int iI = 0; iI < initEs->size(); iI++)
        {
          if(DEBUG) std::cout << "  Initial energy " << iI << " : " << initEs->at(iI)*1000. << std::endl;
        }
      std::cout << " size edeps : " << energyDepositions->size() << std::endl;
      
    }
    
    //event flags
    bool has_2s = false;
    bool has_2s1s = false;

    
    if ( std::find(levels->begin(), levels->end(), "2s") != levels->end() ) 
    {
      has_2s = true;
      /*if( std::find(levels->begin(), levels->end(), "2p_1/2") == levels->end() && std::find(levels->begin(), levels->end(), "2p_3/2") == levels->end() )
      {
        has_2s1s = true;
      }*/
    }
    
    for(unsigned int j = 0; j < initEs->size(); j++ )
    {
      if( initEs->at(j)*1000. > 1630 && initEs->at(j)*1000. < 1650)
      {
        has_2s1s = true;
        if(DEBUG) std::cout << "2s1s transition" << std::endl;
        for(unsigned int iDet = 0; iDet < NDET; iDet++)
        {
          if(DEBUG) std::cout << "  Det " << detectors[iDet] << " energy deposition " << energyDepositions->at(detectors[iDet])*1000. << std::endl;
        }
        for(unsigned int iI = 0; iI < initEs->size(); iI++)
        {
          if(DEBUG) std::cout << "  Initial energy " << iI << " : " << initEs->at(iI)*1000. << std::endl;
        }
      }
    }
    
    
        
    
    //make it keV and apply E resolutions
    for( unsigned int j = 0; j < initEs->size(); j ++)
    {
      initEs->at(j) = initEs->at(j)*1000.;
      hEInit->Fill(initEs->at(j));
    }
    
    for( unsigned int j = 0; j < NDET; j ++)
    {
      if(energyDepositions->at(detectors[j]) > 0)
      {
        energyDepositions->at(detectors[j]) = random_gen->Gaus(energyDepositions->at(detectors[j])*1000.,resolution/2.35);
      }
    }
    
    //free running E spectra
    for( unsigned int j = 0; j < NDET; j ++)
    {
    
      double energy = energyDepositions->at(detectors[j]); 
      if(energy>0.)
      {
        hEdep->Fill(energy,j+1);
        if(has_2s)hEdep2s->Fill(energy,j+1);
        if(has_2s1s) { hEdep2s1s->Fill(energy,j+1); }
      }
      
      //look for coincidences
      if( energy > transition_energies["3p2s"]-1.5*resolution && energy < transition_energies["3p2s"]+1.5*resolution  )
      {
        for( unsigned int iDet = 0; iDet < NDET; iDet++ )
        {
          double coinc_energy = energyDepositions->at(detectors[iDet]);
          if( iDet != j && coinc_energy > 10. )
          {
            unsigned int diff = std::min(abs(j-iDet),NDET-abs(j-iDet));
            hE3p2s->Fill(coinc_energy,diff);
            if(has_2s) hE3p2sHas2s->Fill(coinc_energy,diff);
          }
        }
      }
      if( energy > transition_energies["4p2s"]-1.5*resolution && energy < transition_energies["4p2s"]+1.5*resolution  )
      {
        for( unsigned int iDet = 0; iDet < NDET; iDet++ )
        {
          double coinc_energy = energyDepositions->at(detectors[iDet]);
          if( iDet != j && coinc_energy > 10. )
          {
            unsigned int diff = std::min(abs(j-iDet),NDET-abs(j-iDet));
            hE4p2s->Fill(coinc_energy,diff);
            if(has_2s) hE4p2sHas2s->Fill(coinc_energy,diff);
          }
        }
      }
      if( energy > transition_energies["5p2s"]-1.5*resolution && energy < transition_energies["5p2s"]+1.5*resolution  )
      {
        for( unsigned int iDet = 0; iDet < NDET; iDet++ )
        {
          double coinc_energy = energyDepositions->at(detectors[iDet]);
          if( iDet != j && coinc_energy > 10. )
          {
            unsigned int diff = std::min(abs(j-iDet),NDET-abs(j-iDet));
            hE5p2s->Fill(coinc_energy,diff);
            if(has_2s) hE5p2sHas2s->Fill(coinc_energy,diff);
          }
        }
      }             
    }//detector loop
        
  }
  timer.Stop();
  std::cout << " Looping through tree took " ;
  timer.Print();
  cout << std::endl;
}




void InitHistograms()
{
  hEInit = new TH1D("hEInit","Initial Energy for events with energy depositions; Energy (keV)",4000,0,4000);

  
  hEdep = new TH2F("hEdep","Energy deposition; Energy (keV); Detector",4000,0,4000,NDET,0.5,NDET+0.5);
  hEdep2s = new TH2F("hEdep2s","Energy deposition, cascade passing through 2s; Energy (keV); Detector",4000,0,4000,NDET,0.5,NDET+0.5);
  hEdep2s1s = new TH2F("hEdep2s1s","Energy deposition, 2s1s transition; Energy (keV); Detector",4000,0,4000,NDET,0.5,NDET+0.5);
  
  
  for(unsigned int iDet = 0; iDet < NDET; iDet++)
  {
    hEdep->GetYaxis()->SetBinLabel(iDet+1,detectors[iDet].c_str());
    hEdep2s->GetYaxis()->SetBinLabel(iDet+1,detectors[iDet].c_str());
    hEdep2s1s->GetYaxis()->SetBinLabel(iDet+1,detectors[iDet].c_str());
  }
  
  
  hE3p2s = new TH2F("hE3p2s","Energy deposition 3p2s coincidence ; Energy(KeV) ; Detector difference ",4000,0,4000,NDET/2,0.5,0.5+NDET/2);
  hE4p2s = new TH2F("hE4p2s","Energy deposition 4p2s coincidence ; Energy(KeV) ; Detector difference ",4000,0,4000,NDET/2,0.5,0.5+NDET/2);
  hE5p2s = new TH2F("hE5p2s","Energy deposition 5p2s coincidence ; Energy(KeV) ; Detector difference ",4000,0,4000,NDET/2,0.5,0.5+NDET/2);
  
  hE3p2sHas2s = new TH2F("hE3p2sHas2s","Energy deposition 3p2s coincidence ; Energy(KeV) ; Detector difference ",4000,0,4000,NDET/2,0.5,0.5+NDET/2);
  hE4p2sHas2s = new TH2F("hE4p2sHas2s","Energy deposition 4p2s coincidence ; Energy(KeV) ; Detector difference ",4000,0,4000,NDET/2,0.5,0.5+NDET/2);
  hE5p2sHas2s = new TH2F("hE5p2sHas2s","Energy deposition 5p2s coincidence ; Energy(KeV) ; Detector difference ",4000,0,4000,NDET/2,0.5,0.5+NDET/2);
  
 
}

void FillCanvas()
{
    
  //cDummyHits = new TCanvas("cDummyHits","escaping gamma's line by line",100,100,1100,1300);
  
  
}

int Process()
{

  char input_fname[1024];
  char output_fname[1024];
  std::cout << "Give input filename:" << std::endl;
  std::cin >> input_fname;
  std::cout << "Give output filename:" << std::endl;
  std::cin >> output_fname;
  
  infile = TFile::Open(input_fname, "READ"); 
  if ( infile->IsZombie() ) { std::cout << " could not open input file " << std::endl; return 1; }
  
  outfile = TFile::Open(output_fname, "RECREATE"); 
  if ( outfile->IsZombie() ) { std::cout << " could not open output file " << std::endl; return 1; }
    
  InitHistograms();
  
  runTree = (TTree*)infile->FindObjectAny("RunTree");
  runTree->SetBranchAddress("nEvents",&NInitEvents);
  runTree->GetEntry(1);
  
  tree = (TTree*)infile->FindObjectAny("EventTree");
  LoopTree();
  FillCanvas();
  
  TH1D* h = (TH1D*)infile->FindObjectAny("hEPrimary");
  h->SetDirectory(0);
  h->Write();
  
  infile->Close();
  outfile->Write();
  outfile->Close();
  
  
    
  return 0;
}
