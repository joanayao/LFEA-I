
// O ROOT vai usar esta função com os parâmetros em par para fazer o fit
double predefinedGaussian(double *x, double *par) {
        // par[0]: amplitude
        // par[1]: mean
        // par[2]: standard deviation
        return par[0] * TMath::Gaus(x[0], par[1], par[2]);
}


void Fit_me() {

    //INICIALIZACOES
    TApplication App("A", nullptr, nullptr);
    TCanvas C("C","",1200,800);
    TH1D* histogram = new TH1D("histogram", "Uniformly Distributed Bins", 1024, 0, 1023);
    
    std::vector<double> xData;
    std::vector<double> yData;

    //Leitura do Ficheiro
    std::ifstream file("aml65s4.txt"); // -> Mexer Aqui
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
    histogram->SetTitle("Pico Para Calibracao Americio"); // -> MEXER AQUI
    histogram->SetMarkerColor(kBlue-2);
    histogram->SetMarkerStyle(20);
    histogram->GetXaxis()->SetTitle("Bin");
    histogram->GetYaxis()->SetTitle("N de Contagens"); 

    //BINS DO PICO -> MEXER AQUI
    double min = 699;
    double max = 720;
    histogram->GetXaxis()->SetRangeUser(min, max);

    // Definir os erros como Sqrt(N)
    for(int i = min; i <= max; i++)
        histogram->SetBinError(i,sqrt(yData[i]));

    histogram->SetStats(0);
    histogram->Draw();

    // Criamos uma instância "fitadora"
    TF1 *fitFunc = new TF1("fitFunc", predefinedGaussian, min, max, 3);

    // Parâmetros Iniciais Estimados -> MEXER AQUI
    double amplitude = 7000;
    double mean = 702.5;
    double stddev = 2;

    // Vai dar os parâmtros à nossa função de fit
    fitFunc->SetParameters(amplitude, mean, stddev);

    // Fit em que se ignoram os bins sem nada
    TFitResultPtr fitResult = histogram->Fit(fitFunc, "S");

    double fittedAmplitude = fitFunc->GetParameter(0);
    double fittedMean = fitFunc->GetParameter(1);
    double fittedStdDev = fitFunc->GetParameter(2);

    double fittedAmplitude_E = fitFunc->GetParError(0);
    double fittedMean_E = fitFunc->GetParError(1);
    double fittedStdDev_E = fitFunc->GetParError(2);

    //Parâmetros da Calibração
    double ordenada = 8.754;
    double declive = 0.6703;

    double fittedMeanEnergy = (fittedMean-ordenada)/declive;
    double fittedStdDevEnergy = fittedStdDev/declive;

    cout << "Âmplitude: " << fittedAmplitude << endl;
    cout << "Média: " << fittedMean << endl;
    cout << "Média em Energia: " << fittedMeanEnergy << endl;
    cout << "Desvio Padrão: " << fittedStdDev << endl;
    cout << "Desvio Padrão em Energia: " << fittedStdDevEnergy << endl;

    int N_count_pico = 0;

    for (int i = min; i <= max; i++) {
        if (fittedMean - 3*fittedStdDev <= xData[i] <= fittedMean + 3*fittedStdDev) {
            N_count_pico += yData[i];
        }
    }

    cout << "Nº de contagens do Pico: " << N_count_pico << endl;


    fitFunc->Draw("same");

    // Create a TLegend object and set its position
    TLegend* legend = new TLegend(0.6, 0.7, 0.9, 0.9);
  
    // Calculate the reduced chi-squared value
    double chi2_ = fitFunc->GetChisquare();
    int ndf_ = fitFunc->GetNDF();
    double reducedChi2 = chi2_ / ndf_;
  
    //Create a formatted string for the reduced chi-squared value
    TString chi2String = Form("#chi^{2}/ndf = %.2f/%d = %.2f", chi2_, ndf_, reducedChi2);
    TString meanString = Form("Fitted Mean: %.2f #pm %.2f", fittedMean, fittedMean_E);
    TString StdevString = Form("Fitted StdDev: %.2f #pm %.2f", fittedStdDev, fittedStdDev_E);
    TString ampString = Form("Fitted Amplitude: %.2f #pm %.2f", fittedAmplitude, fittedAmplitude_E);
  
    // Add the reduced chi-squared value to the legend
    legend->AddEntry((TObject*)0, chi2String, "");
    legend->AddEntry((TObject*)0, meanString, "");
    legend->AddEntry((TObject*)0, StdevString, "");
    legend->AddEntry((TObject*)0, ampString, "");
  
    // Draw the legend
    legend->Draw();

    C.SaveAs("FIT_Pico_Calibração_Amerício.png"); // -> MEXER AQUI
    C.Update();
    C.WaitPrimitive();


    std::ofstream outputFile("Parâmetros_Pico_Calibração_Amerício.txt"); // -> MEXER AQUI
    if (!outputFile) {
        std::cout << "Failed to open the output file." << std::endl;
    }

    outputFile << "Âmplitude: " << fittedAmplitude << endl;
    outputFile << "Média: " << fittedMean << endl;
    outputFile << "Média em Energia: " << fittedMeanEnergy << endl;
    outputFile << "Desvio Padrão: " << fittedStdDev << endl;
    outputFile << "Desvio Padrão em Energia: " << fittedStdDevEnergy << endl;
    outputFile << "Nº de contagens do Pico: " << N_count_pico << endl;

    int ndf = fitResult->Ndf();
    double chi2 = fitResult->Chi2();

    std::cout << "Reduced Chi2: " << chi2/ndf << std::endl;
    outputFile << "Reduced Chi2: " << chi2/ndf << std::endl;

    outputFile.close();

    std::cout << "File writing completed successfully." << std::endl;

  
}



