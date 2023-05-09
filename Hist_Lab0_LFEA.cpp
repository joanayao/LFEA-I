
void Hist_Lab0_LFEA() {

    TApplication App("A", nullptr, nullptr);
    TCanvas C("C","",1200,800);

    TH1F* histogram = new TH1F("histogram", "Uniformly Distributed Bins", 100, 0, 1023);

    std::vector<double> xData;
    std::vector<double> yData;

    std::ifstream file("HISTT2.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << "HISTT2.txt" << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        double x, y;
        char comma;
        std::stringstream ss(line);
        ss >> x >> comma >> y;
        xData.push_back(x);
        yData.push_back(y);
    }
    
    file.close();

    //Adding points to graph
    for (int i=0; i<xData.size(); i++) {

        histogram->Fill(xData[i], yData[i]);

    }


    histogram->SetTitle("Amplitude dos Pulsos");
    histogram->SetMarkerColor(kBlue-2);
    histogram->GetXaxis()->SetTitle("Amplitude");
    histogram->GetYaxis()->SetTitle("N de Contagens");
    histogram->Draw();

    TF1* fitFunc = new TF1("fitFunc", "gaus", 0, 10);
    TFitResultPtr fitResult = histogram->Fit(fitFunc, "S");
    fitFunc->Draw("same");

    C.Update();
    C.WaitPrimitive();

}
