void ver_bis() {

    //INICIALIZACOES
    TApplication App("A", nullptr, nullptr);
    TCanvas C("C","",1200,800);
    TH1D* histogram = new TH1D("histogram", "Uniformly Distributed Bins", 1024, 0, 1024);
    
    std::vector<double> xData;
    std::vector<double> yData;

    std::ifstream file("bis600s.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << "RUIDO.CESIO.txt" << std::endl;
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

    //HISTOGRAM
    //Adding points to graph
    for (int i=0; i<xData.size(); i++) {
        histogram->Fill(xData[i], yData[i]);
    }

    histogram->SetTitle("BIS"); // -> MEXER AQUI
    histogram->SetMarkerColor(kBlue-2);
    histogram->SetMarkerStyle(20);
    histogram->GetXaxis()->SetTitle("Bin");
    histogram->GetYaxis()->SetTitle("N de Contagens");
        

    //BINS DO PICO -> MEXER AQUI
    double min = 110;
    double max = 130;

    histogram->GetXaxis()->SetRangeUser(min, max);

    // Definir os erros como Sqrt(N)
    for(int i = min; i <= max; i++)
        histogram->SetBinError(i,sqrt(yData[i]));


    histogram->Draw();
    
    histogram->SetStats(0);
    TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);

    legend->AddEntry(histogram, "Histograma de contagens", "l");
    legend->Draw();


    C.SaveAs("VER_PIQUINHO_bis.png"); // -> MEXER AQUI
    C.Update();
    C.WaitPrimitive();

    std::cout << "File writing completed successfully." << std::endl;

  
}



