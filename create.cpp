int create() {


  TFile * extract = new TFile("extract.root", "READ");
  TH1D* template_T5Z2 = (TH1D*) extract->Get("PT");
  template_T5Z2->Draw();

  TList * li = new TList();
  TList * lis = new TList();
  TList * lis1 = new TList();
  TList * lis2 = new TList();
  TList * nlis1 = new TList();
  TList * nlis2 = new TList();
  TFile * file = new TFile("testTlist.root", "RECREATE");
  TH1D * hist = (TH1D*)template_T5Z2->Clone();
  //TH1D * hist1 = new TH1D("hist1", "hist1", 100, 0, 10);
  //TH1D * hist2 = new TH1D("hist2", "hist2", 100, 0, 10);
  //TH1D * hist3 = new TH1D("hist3", "hist3", 100, 0, 10);
  nlis1->Add(hist);
  nlis1->Add(hist);
  nlis1->Add(hist);

  nlis2->Add(hist);
  nlis2->Add(hist);
  nlis2->Add(hist);

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
