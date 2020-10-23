int test () {

  TList * li = new TList();
  TList * lis = new TList();
  TList * lis1 = new TList();
  TList * lis2 = new TList();
  TList * nlis1 = new TList();
  TList * nlis2 = new TList();
  TFile * file = new TFile("testTlist.root", "RECREATE");
  TH1D * hist = new TH1D("hist", "hist", 100, 0, 10);
  TH1D * histptr = &hist;
  TH1D * hist1 = new TH1D("hist1", "hist1", 100, 0, 10);
  TH1D * hist2 = new TH1D("hist2", "hist2", 100, 0, 10);
  TH1D * hist3 = new TH1D("hist3", "hist3", 100, 0, 10);
  for (int ndata=0; ndata<1000; ndata++) { 
    hist->Fill(1.);}
  for (int ndata=0; ndata<1000; ndata++) {hist1->Fill(2.);}
  for (int ndata=0; ndata<1000; ndata++) {hist2->Fill(3.);}
  for (int ndata=0; ndata<1000; ndata++) {hist3->Fill(4.);}
  //hist1->FillRandom("gaus", 1000);
  //hist2->FillRandom("gaus", 1000);
  //hist3->FillRandom("gaus", 1000);
  nlis1->Add(histptr);
  nlis1->Add(hist1);
  nlis1->Add(hist2);

  nlis2->Add(hist3);
  nlis2->Add(hist2);
  nlis2->Add(hist1);

  lis->Add(nlis1);
  lis->Add(nlis2);
  lis1->Add(nlis1);
  lis1->Add(nlis2);
  lis2->Add(nlis1);
  lis2->Add(nlis2);

  li->Add(lis);
  li->Add(lis1);
  li->Add(lis2);

  hist->Write();
  li->Write("li", 1);
  lis->Write("lis", 1);
  nlis1->Write("nlis1", 1);
  nlis2->Write("nlis2", 1);
  file->Close();

  return 0;

}
