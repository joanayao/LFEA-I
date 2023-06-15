
// O ROOT vai usar esta função com os parâmetros em par para fazer o fit
double predefinedGaussian(double *x, double *par) {
        // par[0]: amplitude
        // par[1]: mean
        // par[2]: standard deviation
        return par[0] * TMath::Gaus(x[0], par[1], par[2]) + par[3]*x[0] + par[4];
}


void pico_bis2() {

    //INICIALIZACOES
    TApplication App("A", nullptr, nullptr);
    TCanvas C("C","",1200,800);
    TH1D* histogram = new TH1D("histogram", "Uniformly Distributed Bins", 1024, 0, 1024);
    
    std::vector<double> xData;
    std::vector<double> yData;

    std::ifstream file("binew600.txt");
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

    histogram->SetTitle("PICO BIS"); // -> MEXER AQUI
    histogram->SetMarkerColor(kBlue-2);
    histogram->SetMarkerStyle(20);
    histogram->GetXaxis()->SetTitle("Bin");
    histogram->GetYaxis()->SetTitle("N de Contagens");
        

    //BINS DO PICO -> MEXER AQUI
    double min = 80;
    double max = 116;

    histogram->GetXaxis()->SetRangeUser(min, max);

    // Definir os erros como Sqrt(N)
    for(int i = min; i <= max; i++)
        histogram->SetBinError(i,sqrt(yData[i]));
    histogram->Draw();

    // Criamos uma instância "fitadora"
    TF1 *fitFunc = new TF1("fitFunc", predefinedGaussian, 110, 130, 5);
    fitFunc->SetRange(min, max);

    // Parâmetros Iniciais Estimados -> MEXER AQUI
    double amplitude = 10000;
    double mean = 103.6;
    double stddev = 0.858;
    double declive=-10;
    double ordenada = 180;

    // Vai dar os parâmtros à nossa função de fit
    fitFunc->SetParameters(amplitude, mean, stddev, declive, ordenada);

    // Fit em que se ignoram os bins sem nada
    histogram->Fit(fitFunc, "W");

    double fittedAmplitude = fitFunc->GetParameter(0);
    double fittedMean = fitFunc->GetParameter(1);
    double fittedStdDev = fitFunc->GetParameter(2);
    double fitteddeclive = fitFunc->GetParameter(3);
    double fittedordenada = fitFunc->GetParameter(4);

    TFitResultPtr fitResult = histogram->Fit(fitFunc, "S");
    double chi = fitResult->Chi2();
    int ndf = fitResult->Ndf();
    double chi_ndf = chi/ndf;

    // Nº de contagens Pico
    int N_count_pico = 0;
    for (int i = min; i <= max; i++) {
        if (fittedMean - 3*fittedStdDev <= xData[i] <= fittedMean + 3*fittedStdDev) {
            N_count_pico += yData[i];
        }
    }

    
        

    cout << "Âmplitude: " << fittedAmplitude << endl;
    cout << "Média: " << fittedMean << endl;
    cout << "Desvio Padrão: " << fittedStdDev << endl;
    cout << "Declive: " << fitteddeclive << endl;
    cout << "Ordenada: " << fittedordenada << endl;
    cout << "Nº de contagens do Pico: " << N_count_pico << endl;
    cout << "chi/ndf: " << chi_ndf << endl;
    cout << "Nº de contagens do Pico: " << N_count_pico << endl;


    
    histogram->SetStats(0);
    TLegend *legend = new TLegend(0.7, 0.7, 0.9, 0.9);
    legend->AddEntry(fitFunc, "Fit pico cesio", "l");
    legend->AddEntry(histogram, "Histograma de contagens", "l");
    legend->Draw();
    

    fitFunc->Draw("same");

    C.SaveAs("PICO_maior_BIS.png"); // -> MEXER AQUI
    C.Update();
    C.WaitPrimitive();


    std::ofstream outputFile("Parâmetros_mior_BIS.txt"); // -> MEXER AQUI
    if (!outputFile) {
        std::cout << "Failed to open the output file." << std::endl;
    }

    outputFile << "Âmplitude: " << fittedAmplitude << endl;
    outputFile << "Média: " << fittedMean << endl;
    outputFile << "Desvio Padrão: " << fittedStdDev << endl;
    outputFile << "Nº de contagens do Pico: " << N_count_pico << endl;

    outputFile.close();

    std::cout << "File writing completed successfully." << std::endl;

  
}



