// O ROOT vai usar esta função com os parâmetros em par para fazer o fit
double predefinedGaussian(double *x, double *par) {
        // par[0]: amplitude
        // par[1]: mean
        // par[2]: standard deviation
        return par[0] * TMath::Gaus(x[0], par[1], par[2]);
}


void desco1_taxacontagem() {

    //INICIALIZACOES
    TApplication App("A", nullptr, nullptr);
    TCanvas C("C","",1200,800);
    TH1D* histogram = new TH1D("histogram", "Uniformly Distributed Bins", 1024, 0, 1023);
    
    std::vector<double> xData;
    std::vector<double> yData;

    //Leitura do Ficheiro
    std::ifstream file("desco2_sem_ruido.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << std::endl;
    }

    double tempo = 456;
    
    //Conversão Bin - Energia
    std::string line;
    while (std::getline(file, line)) {
        double bin, cont, energy;
        std::stringstream ss(line);
        ss >> bin >> cont;
        xData.push_back(bin);
        if (cont < 0) {
            yData.push_back(0);
        }
        else {
        yData.push_back(cont/tempo);
        }
    }
   
    file.close();

    //HISTOGRAM

    //Adding points to graph
    for (int i=0; i<xData.size(); i++) {
        histogram->Fill(xData[i], yData[i]);
    }

    histogram->SetTitle("Fonte desconhecida mais longe");
    histogram->SetMarkerColor(kBlue-2);
    //histogram->SetMarkerStyle(20);
    histogram->GetXaxis()->SetTitle("Bin");
    histogram->GetYaxis()->SetTitle("N de Contagens por segundo");
       

    //BINS DO PICO
    double min = 0;
    double max = 400;
    histogram->GetXaxis()->SetRangeUser(min, max);

    // Definir os erros como Sqrt(N)
    for(int i = min; i <= max; i++)
        histogram->SetBinError(i,sqrt(yData[i]*tempo)/tempo);

    histogram->Draw();

    C.SetLogy();
    C.Update();
    C.SaveAs("DESCO2_plot.png");
    C.WaitPrimitive();

    /*double roi=0;
    for (int i=0; i<(max-min); i++) {
        roi+=yData[min+i];
    }
 
    // Criamos uma instância "fitadora"
    /*TF1 *fitFunc = new TF1("fitFunc", predefinedGaussian, min, max, 3);

    // Parâmetros Iniciais Estimados
    /*PICO 4 bins234-286
    double amplitude = 1443;
    double mean = 252;
    double stddev = 8.2;
    */

    /*//PICO3 BINS 205-230
    double amplitude = 596;
    double mean = 209.7;
    double stddev = 11.12;
    */
    

    /*//PICO2 bins 60 a 73
    double amplitude = 5485;
    double mean = 66.48;
    double stddev = 2.1;
    */

    /*//PICO1  bin 23-37 
    double amplitude = 24818;
    double mean = 29;
    double stddev = 1.5;
    */

    // Vai dar os parâmtros à nossa função de fit
    /*fitFunc->SetParameters(amplitude, mean, stddev);

    // Fit em que se ignoram os bins sem nada
    histogram->Fit(fitFunc, "W");

    double fittedAmplitude = fitFunc->GetParameter(0);
    double fittedMean = fitFunc->GetParameter(1);
    double fittedStdDev = fitFunc->GetParameter(2);

int N_count_pico = 0;

    for (int i = min; i <= max; i++) {
        if (fittedMean - 3*fittedStdDev <= xData[i] <= fittedMean + 3*fittedStdDev) {
            N_count_pico += yData[i];
        }
    }

    cout << "Nº de contagens do Pico: " << N_count_pico << endl;
    //Parâmetros da Calibração
    double ordenada = 8.754;
    double declive = 0.6703;

    double fittedMeanEnergy = (fittedMean-ordenada)/declive;
    double fittedStdDevEnergy = fittedStdDev/declive;

    cout << "Amplitude: " << fittedAmplitude << endl;
    cout << "Média: " << fittedMean << endl;
    cout << "Desvio padrão: " << fittedStdDev << endl;
    cout << "ROI: " << roi << endl;
    cout << "Média em Energia: " << fittedMeanEnergy << endl;
    cout << "Desvio Padrão em Energia: " << fittedStdDevEnergy << endl;
    cout << "Nº de contagens do Pico: " << N_count_pico << endl;

    fitFunc->Draw("same");
    C.SaveAs("DESCO1_PICO4_FIT.png");
    C.Update();
    C.WaitPrimitive();

    std::ofstream outputFile("Parâmetros_Pico4_Absorção_Desco1.txt"); // -> MEXER AQUI
    if (!outputFile) {
        std::cout << "Failed to open the output file." << std::endl;
    }

    outputFile << "Âmplitude: " << fittedAmplitude << endl;
    outputFile << "Média: " << fittedMean << endl;
    outputFile << "Média em Energia: " << fittedMeanEnergy << endl;
    outputFile << "Desvio Padrão: " << fittedStdDev << endl;
    outputFile << "Desvio Padrão em Energia: " << fittedStdDevEnergy << endl;
    outputFile << "Nº de contagens do Pico: " << N_count_pico << endl;

    outputFile.close();

    std::cout << "File writing completed successfully." << std::endl;*/

  
}
