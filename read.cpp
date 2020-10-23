// copied and modified from BatCommons/pulse/PulseTools.h/cxx
vector<double> TH1D2Vector(const TH1D* aHisto)
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

// copied and modified from BatCommons/pulse/PulseTools.h/cxx
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
// makes a nestedvector from a Tlist
vector<vector<vector<double>>> TList2NestedVector(const TList* aTList)
{

  TH1D* aHist;
  vector<vector<double>> tempVectorlv1;
  vector<vector<vector<double>>> tempVectorlv2;
  int counta = 0, countb = 0;

  while (aTList->At(counta)) {
    TList* tempTList = (TList*) aTList->At(counta);
    while (tempTList->At(countb)) {
      aHist = (TH1D*) tempTList->At(countb);
      tempVectorlv1.push_back(TH1D2Vector(aHist));
      countb++;
    }
    tempVectorlv2.push_back(tempVectorlv1);
    countb = 0; // refresh counter
    tempVectorlv1.clear(); // refresh tempVector holder
    counta++;
  }

  return tempVectorlv2;
}

//Oct 17: 
TList* NestedVector2TList(const vector<vector<vector<double>>>& tempVectorlv2, const string& aHistoName,
                          const double xscale) {
  //Note we do not check for zero length here because sometime these vectors are zero on purpose

  TList* tempTList[tempVectorlv2.size()];
  TList *aTList = new TList();
  TH1D* aHist;
  int counta = 0;
  int countb = 0;
  string histname;
  
  while (counta != tempVectorlv2.size()) {
    tempTList[counta] = new TList();
    while (countb != tempVectorlv2[0].size()) {
      histname = aHistoName + to_string(counta) + "_" + to_string(countb); // construct hist name
      aHist = Vector2TH1D(tempVectorlv2[counta][countb], histname, xscale);
      tempTList[counta]->Add(aHist); // add hist to lv1 TList
      countb++;
    }
    aTList->Add(tempTList[counta]);
    countb = 0;
    counta++;
  }
  
  return aTList;
}

void read() {

  TFile * file = new TFile("testTlist.root", "READ");
  TList * lis = (TList*)file->Get("lis");

  vector<vector<vector<double>>> vec = TList2NestedVector(lis);

  //cout << vec.size() << " " << vec[0].size() << " " << vec[0][0].size() << endl;

  // testing to write Vector to TH1D
/*  TH1D histoutput = Vector2TH1D(vec[1][0], "hist", 0.1);
  cout << "data" << histoutput.GetBinContent(41) << endl;
  TFile * savefile = new TFile("saveFile.root", "NEW");
  histoutput.Write();
  histoutput.Draw();
  savefile->Close();
*/

  // NestedVector2TList
  TList* aa = NestedVector2TList(vec, "hist", 0.1);

  // comparing two vectors
  //vector<vector<vector<double>>> vec_back = TList2NestedVector(aa);
  //vec == vec_back ? cout << "y" << endl : cout << "n" << endl;

  // validation 
/*  for (int xxxx=0; xxxx<vec.size(); xxxx++) {
    for (int xxx=0; xxx<vec[0].size(); xxx++) {
      for (int xx=0; xx<vec[0][0].size(); xx++) {
        cout<< xx << "  " << vec[1][0][xx] <<"  " << vec_back[1][0][xx] << "  "  << endl; }}}
*/

  // method to extract TH1D from TList
/*  TList* bb = (TList*) aa->At(0);
  TH1D* hh = (TH1D*) bb->At(0);
  hh->Draw();
*/

  // method to write TList to file
/*  TFile * savefile = new TFile("saveFile.root", "RECREATE");
  //aa->Print();
  aa->Write("aa", 1);
  // hh->Write();
  savefile->Close();
*/

}
