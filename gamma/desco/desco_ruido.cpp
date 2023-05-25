
void desco_ruido() {

    TApplication App("A", nullptr, nullptr);
    TCanvas C("C","",1200,800);

    TH1F* histogram = new TH1F("histogram", "Uniformly Distributed Bins", 300, 0, 1023);
    TH1F* histograma = new TH1F("histograma", "Uniformly Distributed Bins", 300, 0, 1023);
    TH1F* subtracao = new TH1F("histograma", "Uniformly Distributed Bins", 300, 0, 1023);

    std::vector<double> xData;
    std::vector<double> yData;
    std::vector<double> xdata;
    std::vector<double> ydata;
    std::vector<double> sub;
    double valor_sub;

    std::ifstream file("ruido456.txt");
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

    std::ifstream files("desco2.txt");
    if (!files.is_open()) {
        std::cerr << "Error opening file: " << "COM.FONTE.txt" << std::endl;
        return;
    }
    
    while (std::getline(files, line)) {
        double a, b;
        char co;
        std::stringstream ss(line);
        ss >> a >> co >> b;
        xdata.push_back(a);
        ydata.push_back(b);
    }

    
    
    files.close();

    //Adding points to graph
    for (int i=0; i<xData.size(); i++) {

        histogram->Fill(xData[i], yData[i]);

    }

    //Adding points to graph
    for (int i=0; i<xdata.size(); i++) {

        histograma->Fill(xData[i], ydata[i]);

    }

    //Creating points for subtraction
    for (int i=0; i<xdata.size(); i++) {
        valor_sub=ydata[i]-yData[i];
        sub.push_back(valor_sub);
        //cout<< sub[i] << endl;

    }

    std::ofstream outputFile("desco2_sem_ruido.txt");
    if (!outputFile) {
        std::cout << "Failed to open the output file." << std::endl;
    }

    for (size_t i = 0; i < sub.size(); ++i) {
        outputFile << i << " " << sub[i] << std::endl;
    }

    outputFile.close();

    std::cout << "File writing completed successfully." << std::endl;


    //Adding points to graph
    for (int i=0; i<xdata.size(); i++) {
        subtracao->Fill(xData[i], sub[i]);

    }

    histogram->SetStats(0);
    histograma->SetStats(0);
    subtracao->SetStats(0);

    //criar legendas para os gráficos
    TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);  // (x1, y1, x2, y2)


    histograma->SetTitle("Erro sistematico - Radiacao ambiente");
    histograma->SetLineColor(kRed);
    histogram->SetLineColor(kGreen);
    histogram->SetLineWidth(4);
    histograma->SetLineWidth(4);
    histograma->GetXaxis()->SetTitle("Bins");
    histograma->GetYaxis()->SetTitle("N de Contagens");
    histogram-> SetFillColor(kGreen);
    histograma-> SetFillColor(kRed);
    histograma->Draw("Hist");
    histogram->Draw("SAME HIST");
    subtracao->SetLineColor(kBlue-2);
    subtracao->SetLineWidth(4);
    subtracao->Draw("SAME HIST");

    legend->AddEntry(histogram, "Contagens sem fonte", "l");  // (object, label, option)
    legend->AddEntry(histograma, "Contagens com fonte", "l");  // (object, label, option)
    legend->AddEntry(subtracao, "Subtracao (com fonte-sem fonte)", "l");  // (object, label, option)
    legend->Draw();

    /*TF1* fitFunc = new TF1("fitFunc", "gaus", 0, 10);
    TFitResultPtr fitResult = histogram->Fit(fitFunc, "S");
    fitFunc->Draw("same");*/

    C.SaveAs("desco_ruido.png");

    C.Update();
    C.WaitPrimitive();

}
