double predefinedGaussian(double *x, double *par) {
        // par[0]: amplitude_1
        // par[1]: mean_1
        // par[2]: standard_deviation_1
        // par[3]: amplitude_2
        // par[4]: mean_2
        // par[5]: standard_deviation_2
        // par[6]: amplitude_3
        // par[7]: mean_3
        // par[8]: standard_deviation_3

        return par[0] * TMath::Gaus(x[0], par[1], par[2]) + par[3] * TMath::Gaus(x[0], par[4], par[5]);
    
}


void fit_2_bis() {

    //INICIALIZACOES
    TApplication App("A", nullptr, nullptr);
    TCanvas C("C","",1200,800);
    TH1D* histogram = new TH1D("histogram", "Uniformly Distributed Bins", 1024, 0, 1023);
    
    std::vector<double> xData;
    std::vector<double> yData;

    //Leitura do Ficheiro
    std::ifstream file("bis600s.txt"); // -> Mexer Aqui
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << std::endl;
    }
    
    //Leitura do Ficheiro
    std::string line;
    while (std::getline(file, line)) {

        double bin, cont, trash;
        char comma;

        std::stringstream ss(line);
        ss >> bin >> comma >> cont >> comma >> trash;

        xData.push_back(bin);
        if (cont < 0) {
            yData.push_back(0);
        }
        else {
        yData.push_back(cont);
        }
    }
   
    file.close();

    //HISTOGRAM

    //Adding points to graph
    for (int i=0; i<xData.size(); i++) {
        histogram->Fill(xData[i], yData[i]);
    }

    //histogram->SetBinErrorOption(TH1::kNormal);
    histogram->SetTitle("Americio + Ar - Fit 3 Gaussianas"); // -> MEXER AQUI
    histogram->SetMarkerColor(kBlue-2);
    histogram->SetMarkerStyle(20);
    histogram->GetXaxis()->SetTitle("Bin");
    histogram->GetYaxis()->SetTitle("N de Contagens"); 

    //BINS DO PICO -> MEXER AQUI
    double min = 118;
    double max = 126;
    histogram->GetXaxis()->SetRangeUser(min, max);

    // Definir os erros como Sqrt(N)
    for(int i = min; i <= max; i++)
        histogram->SetBinError(i,sqrt(yData[i]));

    histogram->SetStats(0);
    histogram->Draw();

    // Criamos uma instância "fitadora"
    TF1 *fitFunc = new TF1("fitFunc", predefinedGaussian, min, max, 9);

    // Parâmetros Iniciais Estimados -> MEXER AQUI
    double amplitude_1 = 350;
    double mean_1 = 120;
    double stddev_1 = 1;
    double amplitude_2 = 200;
    double mean_2 = 122;
    double stddev_2 = 2;


    // Vai dar os parâmtros à nossa função de fit
    fitFunc->SetParameters(amplitude_1, mean_1, stddev_1, amplitude_2, mean_2, stddev_2);
    //fitFunc->FixParameter(4, 584);
    //fitFunc->FixParameter(5, 7);
    //fitFunc->FixParameter(7, 574);




    // Fit
    histogram->Fit(fitFunc, "S");

    double fittedAmplitude_1 = fitFunc->GetParameter(0);
    double fittedMean_1 = fitFunc->GetParameter(1);
    double fittedStdDev_1 = fitFunc->GetParameter(2);
    double fittedAmplitude_2 = fitFunc->GetParameter(3);
    double fittedMean_2 = fitFunc->GetParameter(4);
    double fittedStdDev_2 = fitFunc->GetParameter(5);

    double fittedAmplitude_1_E = fitFunc->GetParError(0);
    double fittedMean_1_E = fitFunc->GetParError(1);
    double fittedStdDev_1_E = fitFunc->GetParError(2);
    double fittedAmplitude_2_E = fitFunc->GetParError(3);
    double fittedMean_2_E = fitFunc->GetParError(4);
    double fittedStdDev_2_E = fitFunc->GetParError(5);


    cout << "Âmplitude 1: " << fittedAmplitude_1 << " +- " << fittedAmplitude_1_E << endl;
    cout << "Média 1: " << fittedMean_1 << " +- " << fittedMean_1_E << endl;
 
    cout << "Desvio Padrão 1: " << fittedStdDev_1 << " +- " << fittedStdDev_1_E << endl;

    cout << "Âmplitude 2: " << fittedAmplitude_2 << " +- " << fittedAmplitude_2_E << endl;
    cout << "Média 2: " << fittedMean_2 << " +- " << fittedMean_2_E << endl;

    cout << "Desvio Padrão 2: " << fittedStdDev_2 << " +- " << fittedStdDev_2_E << endl;

    int N_count_pico_1 = 0;
    int N_count_pico_2 = 0;


    for (int i = min; i <= max; i++) {
        if (fittedMean_1 - fittedStdDev_1 < xData[i] && xData[i] < fittedMean_1 + fittedStdDev_1) {
            N_count_pico_1 += yData[i];
        }
    }

    for (int j = min; j <= max; j++) {
        if (fittedMean_2 - fittedStdDev_2 < xData[j] && xData[j] < fittedMean_2 + fittedStdDev_2) {
            N_count_pico_2 += yData[j];
        }
    }

    cout << "Nº de contagens do Pico 1: " << N_count_pico_1 << endl;
    cout << "Nº de contagens do Pico 2: " << N_count_pico_2 << endl;




    fitFunc->Draw("same");
    C.SaveAs("FIT_2picos_guas_bis.png"); // -> MEXER AQUI
    C.Update();
    C.WaitPrimitive();


    std::ofstream outputFile("Parâmetros_2picosjuntos_bis.txt"); // -> MEXER AQUI
    if (!outputFile) {
        std::cout << "Failed to open the output file." << std::endl;
    }

    outputFile << "Âmplitude 1: " << fittedAmplitude_1 << " +- " << fittedAmplitude_1_E << endl;
    outputFile << "Média 1: " << fittedMean_1 << " +- " << fittedMean_1_E << endl;

    outputFile << "Desvio Padrão 1: " << fittedStdDev_1 << " +- " << fittedStdDev_1_E << endl;

    outputFile << "Nº de contagens do Pico 1: " << N_count_pico_1 << endl;

    outputFile << "Âmplitude 2: " << fittedAmplitude_2 << " +- " << fittedAmplitude_2_E << endl;
    outputFile << "Média 2: " << fittedMean_2 << " +- " << fittedMean_2_E << endl;

    outputFile << "Desvio Padrão 2: " << fittedStdDev_2 << " +- " << fittedStdDev_2_E << endl;

    outputFile << "Nº de contagens do Pico 2: " << N_count_pico_2 << endl;


    double chi2 = fitFunc->GetChisquare();
    int ndf = fitFunc->GetNDF();
    outputFile << "Chi^2/ndf: " << chi2/ndf << endl;
    cout << "CHI CORRIdo " << chi2/ndf << endl;

    outputFile.close();

    std::cout << "File writing completed successfully." << std::endl;

  
}



