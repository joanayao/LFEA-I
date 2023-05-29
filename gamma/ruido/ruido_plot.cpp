
void ruido_plot() {

    TApplication App("A", nullptr, nullptr);
    TCanvas C("C","",1200,800);
    //C.SetLogy();

    TH1F* histogram = new TH1F("histogram", "Uniformly Distributed Bins", 1024, 0, 1023);

    std::vector<double> xData;
    std::vector<double> yData;

    std::ifstream file("ruido20m.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        double x, y, trash;
        char comma;
        std::stringstream ss(line);
        ss >> x >> comma >> y >> comma >> trash;
        xData.push_back(x);
        yData.push_back(y);
    }
    
    file.close();

    //Adding points to graph
    for (int i=0; i<xData.size(); i++) {

        histogram->Fill(xData[i], yData[i]);

    }

    double min = 900;
    double max = 1024;
    histogram->GetXaxis()->SetRangeUser(min, max);

    histogram->SetTitle("Radiacao Ambiente 20 min");
    histogram->SetMarkerColor(kBlue-2);
    histogram->GetXaxis()->SetTitle("Bin");
    histogram->GetYaxis()->SetTitle("N de Contagens");
    histogram->Draw();

    C.SaveAs("Plot_Ruido_20_min_900-1024.png");
    C.Update();
    C.WaitPrimitive();

}
