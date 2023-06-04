
// O ROOT vai usar esta função com os parâmetros em par para fazer o fit
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

        return par[0] * TMath::Gaus(x[0], par[1], par[2]) + par[3] * TMath::Gaus(x[0], par[4], par[5]) + par[6] * TMath::Gaus(x[0], par[7], par[8]);
        
}


void Fit_me() {

    //INICIALIZACOES
    TApplication App("A", nullptr, nullptr);
    TCanvas C("C","",1200,800);
    TH1D* histogram = new TH1D("histogram", "Uniformly Distributed Bins", 1024, 0, 1023);
    
    std::vector<double> xData;
    std::vector<double> yData;

    //Leitura do Ficheiro
    std::ifstream file("am70s3.txt"); // -> Mexer Aqui
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
    histogram->SetTitle("Fonte Semi-Desconhecida - Fit 3 Gaussianas"); // -> MEXER AQUI
    histogram->SetMarkerColor(kBlue-2);
    histogram->SetMarkerStyle(20);
    histogram->GetXaxis()->SetTitle("Bin");
    histogram->GetYaxis()->SetTitle("N de Contagens"); 

    //BINS DO PICO -> MEXER AQUI
    double min = 620;
    double max = 720;
    histogram->GetXaxis()->SetRangeUser(min, max);

    // Definir os erros como Sqrt(N)
    for(int i = min; i <= max; i++)
        histogram->SetBinError(i,sqrt(yData[i]));

    histogram->Draw();

    // Criamos uma instância "fitadora"
    TF1 *fitFunc = new TF1("fitFunc", predefinedGaussian, min, max, 9);

    // Parâmetros Iniciais Estimados -> MEXER AQUI
    double amplitude_1 = 3500;
    double mean_1 = 677;
    double stddev_1 = 6.3;
    double amplitude_2 = 700;
    double mean_2 = 660;
    double stddev_2 = 4.8;
    double amplitude_3 = 100;
    double mean_3 = 640;
    double stddev_3 = 8;

    // Vai dar os parâmtros à nossa função de fit
    fitFunc->SetParameters(amplitude_1, mean_1, stddev_1, amplitude_2, mean_2, stddev_2, amplitude_3, mean_3, stddev_3);

    // Fit
    histogram->Fit(fitFunc, "WE");

    double fittedAmplitude_1 = fitFunc->GetParameter(0);
    double fittedMean_1 = fitFunc->GetParameter(1);
    double fittedStdDev_1 = fitFunc->GetParameter(2);
    double fittedAmplitude_2 = fitFunc->GetParameter(3);
    double fittedMean_2 = fitFunc->GetParameter(4);
    double fittedStdDev_2 = fitFunc->GetParameter(5);
    double fittedAmplitude_3 = fitFunc->GetParameter(6);
    double fittedMean_3 = fitFunc->GetParameter(7);
    double fittedStdDev_3 = fitFunc->GetParameter(8);

    double fittedAmplitude_1_E = fitFunc->GetParError(0);
    double fittedMean_1_E = fitFunc->GetParError(1);
    double fittedStdDev_1_E = fitFunc->GetParError(2);
    double fittedAmplitude_2_E = fitFunc->GetParError(3);
    double fittedMean_2_E = fitFunc->GetParError(4);
    double fittedStdDev_2_E = fitFunc->GetParError(5);
    double fittedAmplitude_3_E = fitFunc->GetParError(6);
    double fittedMean_3_E = fitFunc->GetParError(7);
    double fittedStdDev_3_E = fitFunc->GetParError(8);

    //Parâmetros da Calibração
    double ordenada = 3.71;
    double declive = 0.00263;

    double fittedMeanEnergy_1 = fittedMean_1*declive+ordenada;
    double fittedStdDevEnergy_1 = fittedStdDev_1*declive;
    double fittedMeanEnergy_2 = fittedMean_2*declive+ordenada;
    double fittedStdDevEnergy_2 = fittedStdDev_2*declive;
    double fittedMeanEnergy_3 = fittedMean_3*declive+ordenada;
    double fittedStdDevEnergy_3 = fittedStdDev_3*declive;

    cout << "Âmplitude 1: " << fittedAmplitude_1 << " +- " << fittedAmplitude_1_E << endl;
    cout << "Média 1: " << fittedMean_1 << " +- " << fittedMean_1_E << endl;
    cout << "Média em Energia 1: " << fittedMeanEnergy_1 << endl;
    cout << "Desvio Padrão 1: " << fittedStdDev_1 << " +- " << fittedStdDev_1_E << endl;
    cout << "Desvio Padrão em Energia 1: " << fittedStdDevEnergy_1 << endl;
    cout << "Âmplitude 2: " << fittedAmplitude_2 << " +- " << fittedAmplitude_2_E << endl;
    cout << "Média 2: " << fittedMean_2 << " +- " << fittedMean_2_E << endl;
    cout << "Média em Energia 2: " << fittedMeanEnergy_2 << endl;
    cout << "Desvio Padrão 2: " << fittedStdDev_2 << " +- " << fittedStdDev_2_E << endl;
    cout << "Desvio Padrão em Energia 2: " << fittedStdDevEnergy_2 << endl;
    cout << "Âmplitude 3: " << fittedAmplitude_3 << " +- " << fittedAmplitude_3_E << endl;
    cout << "Média 3: " << fittedMean_3 << " +- " << fittedMean_3_E << endl;
    cout << "Média em Energia 3: " << fittedMeanEnergy_3 << endl;
    cout << "Desvio Padrão 3: " << fittedStdDev_3 << " +- " << fittedStdDev_3_E << endl;
    cout << "Desvio Padrão em Energia 3: " << fittedStdDevEnergy_3 << endl;

    int N_count_pico_1 = 0;
    int N_count_pico_2 = 0;
    int N_count_pico_3 = 0;

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

    for (int k = min; k <= max; k++) {
        if (fittedMean_3 - fittedStdDev_3 < xData[k] && xData[k] < fittedMean_3 + fittedStdDev_3) {
            N_count_pico_3 += yData[k];
            cout << "3-contagem: " << N_count_pico_3 << endl;
        }
    }

    cout << "Nº de contagens do Pico 1: " << N_count_pico_1 << endl;
    cout << "Nº de contagens do Pico 2: " << N_count_pico_2 << endl;
    cout << "Nº de contagens do Pico 3: " << N_count_pico_3 << endl;


    fitFunc->Draw("same");
    C.SaveAs("FIT_3_Picos_Fonte_Desco.png"); // -> MEXER AQUI
    C.Update();
    C.WaitPrimitive();


    std::ofstream outputFile("Parâmetros_3_Picos_Fonte_Desco.txt"); // -> MEXER AQUI
    if (!outputFile) {
        std::cout << "Failed to open the output file." << std::endl;
    }

    outputFile << "Âmplitude 1: " << fittedAmplitude_1 << " +- " << fittedAmplitude_1_E << endl;
    outputFile << "Média 1: " << fittedMean_1 << " +- " << fittedMean_1_E << endl;
    outputFile << "Média em Energia 1: " << fittedMeanEnergy_1 << endl;
    outputFile << "Desvio Padrão 1: " << fittedStdDev_1 << " +- " << fittedStdDev_1_E << endl;
    outputFile << "Desvio Padrão em Energia 1: " << fittedStdDevEnergy_1 << endl;
    outputFile << "Nº de contagens do Pico 1: " << N_count_pico_1 << endl;

    outputFile << "Âmplitude 2: " << fittedAmplitude_2 << " +- " << fittedAmplitude_2_E << endl;
    outputFile << "Média 2: " << fittedMean_2 << " +- " << fittedMean_2_E << endl;
    outputFile << "Média em Energia 2: " << fittedMeanEnergy_2 << endl;
    outputFile << "Desvio Padrão 2: " << fittedStdDev_2 << " +- " << fittedStdDev_2_E << endl;
    outputFile << "Desvio Padrão em Energia 2: " << fittedStdDevEnergy_2 << endl;
    outputFile << "Nº de contagens do Pico 2: " << N_count_pico_2 << endl;

    outputFile << "Âmplitude 3: " << fittedAmplitude_3 << " +- " << fittedAmplitude_3_E << endl;
    outputFile << "Média 3: " << fittedMean_3 << " +- " << fittedMean_3_E << endl;
    outputFile << "Média em Energia 3: " << fittedMeanEnergy_3 << endl;
    outputFile << "Desvio Padrão 3: " << fittedStdDev_3 << " +- " << fittedStdDev_3_E << endl;
    outputFile << "Desvio Padrão em Energia 3: " << fittedStdDevEnergy_3 << endl;
    outputFile << "Nº de contagens do Pico 3: " << N_count_pico_3 << endl;

    double chi2 = fitFunc->GetChisquare();
    int ndf = fitFunc->GetNDF();
    outputFile << "Chi^2/ndf: " << chi2/ndf << endl;

    outputFile.close();

    std::cout << "File writing completed successfully." << std::endl;

  
}



