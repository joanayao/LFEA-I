
// O ROOT vai usar esta função com os parâmetros em par para fazer o fit
double predefinedGaussian(double *x, double *par) {
        // par[0]: amplitude
        // par[1]: mean
        // par[2]: standard deviation
        return par[0] * TMath::Gaus(x[0], par[1], par[2]);
}


void ruido_fit_potassio() {

    //INICIALIZACOES
    TApplication App("A", nullptr, nullptr);
    TCanvas C("C","",1200,800);
    TH1D* histogram = new TH1D("histogram", "Uniformly Distributed Bins", 1024, 0, 1023);
    
    std::vector<double> xData;
    std::vector<double> yData;

    //Leitura do Ficheiro
    std::ifstream file("ruido20m.txt"); // -> Mexer Aqui
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << std::endl;
    }
    
    //Conversão Bin - Energia
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

    histogram->SetTitle("Pico Principal"); // -> MEXER AQUI
    histogram->SetMarkerColor(kBlue-2);
    histogram->SetMarkerStyle(20);
    histogram->GetXaxis()->SetTitle("Bin");
    histogram->GetYaxis()->SetTitle("N de Contagens");

    // Bins do Pico -> MEXER AQUI
    double min = 30;
    double max = 70;
    histogram->GetXaxis()->SetRangeUser(min, max);

    // Definir os erros como Sqrt(N)
    for(int i = min; i <= max; i++)
        histogram->SetBinError(i,sqrt(yData[i]));

    histogram->Draw();

    // Criamos uma instância "fitadora"
    TF1 *fitFunc = new TF1("fitFunc", predefinedGaussian, min, max, 3);

    // Parâmetros Iniciais Estimados -> MEXER AQUI
    double amplitude = 1100;
    double mean = 74;
    double stddev = 20;

    // Vai dar os parâmtros à nossa função de fit
    fitFunc->SetParameters(amplitude, mean, stddev);

    // Fit em que se ignoram os bins sem nada
    histogram->Fit(fitFunc, "W");

    double fittedAmplitude = fitFunc->GetParameter(0);
    double fittedMean = fitFunc->GetParameter(1);
    double fittedStdDev = fitFunc->GetParameter(2);

    //Parâmetros da Calibração
    double ordenada = 9.1;
    double declive = 0.6725;

    double fittedMeanEnergy = (fittedMean-ordenada)/declive;
    double fittedStdDevEnergy = fittedStdDev/declive;

    cout << "Âmplitude: " << fittedAmplitude << " Contagens" << endl;
    cout << "Média: " << fittedMean << " Bin" << endl;
    cout << "Média em Energia: " << fittedMeanEnergy << " keV" << endl;
    cout << "Desvio Padrão: " << fittedStdDev << " Bin" << endl;
    cout << "Desvio Padrão em Energia: " << fittedStdDevEnergy << " keV" << endl;

    int N_count_pico = 0;

    for (int i = min; i <= max; i++) {
        if (fittedMean - 3*fittedStdDev <= xData[i] <= fittedMean + 3*fittedStdDev) {
            N_count_pico += yData[i];
        }
    }

    cout << "Nº de contagens do Pico: " << N_count_pico << " Contagens" << endl;


    fitFunc->Draw("same");
    C.SaveAs("FIT_PICO_Principal.png"); // -> MEXER AQUI
    C.Update();
    C.WaitPrimitive();


    std::ofstream outputFile("Parâmetros_Pico_Principal.txt"); // -> MEXER AQUI
    if (!outputFile) {
        std::cout << "Failed to open the output file." << std::endl;
    }

    outputFile << "Âmplitude: " << fittedAmplitude << " Contagens" << endl;
    outputFile << "Média: " << fittedMean << " Bin" << endl;
    outputFile << "Média em Energia: " << fittedMeanEnergy << " keV" << endl;
    outputFile << "Desvio Padrão: " << fittedStdDev << " Bin" << endl;
    outputFile << "Desvio Padrão em Energia: " << fittedStdDevEnergy << " keV" << endl;
    outputFile << "Nº de contagens do Pico: " << N_count_pico << " Contagens" << endl;

    outputFile.close();

    std::cout << "File writing completed successfully." << std::endl;

  
}



