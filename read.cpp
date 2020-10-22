// copied and modified from BatCommons/pulse/PulseTools.h/cxx
vector<double> TH1D2Vector(TH1D* aHisto)
{

  // number of bins (histrogram x axis)
  int nBins = aHisto->GetNbinsX();
  
  // create and fill vector (remembering that histograms bins start at 1 in ROOT)
  
  vector<double> tempVector;
  for(int binCtr=1; binCtr<=nBins; binCtr++)
  {
     tempVector.push_back(aHisto->GetBinContent(binCtr));
  }
  
  return tempVector;

}

TH1D* Vector2TH1D(const vector<double>& aVector, const string& aHistoName,
                             const double xscale)
{
   //Note we do not check for zero length here because sometime these vectors are zero on purpose

   int nBins = aVector.size();

   //now initialize a histogram, x axis is bins
   string histName = Form("%s", aHistoName.c_str());
   TH1D* tempHistogram = new TH1D(histName.c_str(), histName.c_str(), nBins, 0, ((double)nBins)*xscale);

   //now fill the histogram with the values in each bin of the vector
   //remembering that bins start at 1 in ROOT
   for(int binCtr=1; binCtr<=nBins; binCtr++)
   {
      tempHistogram->SetBinContent(binCtr, aVector[binCtr-1]);
      //cout << binCtr << "  " << aVector[binCtr-1] << endl;
   }

   return tempHistogram;
}


// Oct. 16: write TList2NestedVector, validated. looks good
vector<vector<vector<double>>> TList2NestedVector(const TList* aTList)
{

  TH1D* hist1;
  vector<vector<double>> tempVectorlv1;
  vector<vector<vector<double>>> tempVectorlv2;

  int count = 0;
  int counta = 0;

  while (aTList->At(count)) {
    TList* midli = (TList*) aTList->At(count);
    counta = 0; // refresh counter
    tempVectorlv1.clear(); // refresh tempVector holder
    while (midli->At(counta)) {
      hist1 = (TH1D*) midli->At(counta);
      //hist1->Draw();
      tempVectorlv1.push_back(TH1D2Vector(hist1));
      counta++;
    }
    tempVectorlv2.push_back(tempVectorlv1);
    count++;
  }

  return tempVectorlv2;
}

//Oct 17: 
TList* NestedVector2TList(const vector<vector<vector<double>>>& tempVectorlv2, const string& aHistoName, const double xscale) {
  //Note we do not check for zero length here because sometime these vectors are zero on purpose

  TList* aTListlv1 = new TList();
  //TList * li = new TList();
  //TList *aTListlv1ptr = &aTListlv1;
  TList *aTListlv2 = new TList();
  //TList aTListlv2aa;
  //aTListlv2 = &aTListlv2aa;
  //TH1D aHist;
  TH1D* aHistptr;
  //aHistptr = &aHist;
  int count = 0;
  int counta = 0;
  string histname;
  
  cout << "count "<< count << endl;
  cout << "tempVectorlv2.size() " << tempVectorlv2.size() << endl;
  while (count != tempVectorlv2.size()) {
  //while (count != 1) {
    cout << "count "<< count << endl;
    while (counta != tempVectorlv2[0].size()) {
    //while (counta != 1) {
      histname = aHistoName + to_string(count) + to_string(counta);
      aHistptr = Vector2TH1D(tempVectorlv2[count][counta], histname, xscale);
      cout << aHistptr->GetName() << endl;
      cout << aHistptr->GetBinContent(11) << endl;
      aHistptr->Draw();
      //sleep(10);
      //aHistptr->Draw();
      //aHist.Draw();
      aTListlv1->Add(aHistptr);
      counta++;
      cout << "counta "<< counta << endl;
    }
    aTListlv2->Add(aTListlv1);
    aTListlv1->Print();
    //aTListlv1->Clear();
    aTListlv1->Print();
    counta = 0;
    count++;
  }
  
  //return aTListlv2;
  //aHistptr = &aHist;
  //cout << aHistptr->GetName() << endl;
  //cout << aHistptr->GetBinContent(11) << endl;
  cout << "change to pointer" << endl;
  return aTListlv2;
  //return aTListlv2aa;

}

void read() {

  //gDebug=3;

 
  TFile * file = new TFile("testTlist.root", "READ");
  TList * lis = (TList*)file->Get("lis");
  TList * nlis1 = (TList*)file->Get("nlis1");

  TH1D* hist = (TH1D*) nlis1->At(1); 
  //hist->Draw();
  
  //TList* nli = (TList*) lis->At(0);
  //TH1D* hist1 = (TH1D*) nli->Last();
  //hist1->Draw();
  //TH1D* hist2 = (TH1D*) nli->First();
  //hist2->Draw("same");

  //TH1D* hist1 = TList2NestedVector(lis);
  //vector<double> vec = TH1D2Vector(hist1);
  vector<vector<vector<double>>> vec = TList2NestedVector(lis);

  cout << vec.size() << " " << vec[0].size() << " " << vec[0][0].size() << endl;
//  for (int xx=0; xx<vec[0][0].size(); xx++) {
//    cout<< xx << "  " << vec[1][0][xx] <<"  " << vec[1][1][xx] << "  " << vec[1][2][xx] << endl; }

  //testing to write Vector to TH1D
/*  TH1D histoutput = Vector2TH1D(vec[1][0], "hist", 0.1);
  cout << "data" << histoutput.GetBinContent(41) << endl;
  TFile * savefile = new TFile("saveFile.root", "NEW");
  histoutput.Write();
  histoutput.Draw();
  savefile->Close();
*/
  //testing NestedVector2TList
  TList* aa = NestedVector2TList(vec, "hist", 0.1);
  //TH1D * histoutput = NestedVector2TList(vec, "hist", 0.1);
  //histoutput.Draw();
  TFile * savefile = new TFile("saveFile.root", "RECREATE");
  //if (histoutput) histoutput->Draw();
  cout << "Can I get here?" << endl;
  aa->Print();
  aa->Write("aa", 1);
  //histoutput->Draw();
  savefile->Close();

}

