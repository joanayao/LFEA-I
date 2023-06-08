
// O ROOT vai usar esta função com os parâmetros em par para fazer o fit
double predefinedGaussian(double *x, double *par) {
        // par[0]: amplitude
        // par[1]: mean
        // par[2]: standard deviation
        return par[0] * TMath::Gaus(x[0], par[1], par[2]);
}
void PaintBin (TH1D *histogram, Int_t bin, Int_t color) 
{
   //printf("%d %d %d\n", bin, color, histogram->GetBinContent(bin));
   TBox *b = new TBox(histogram->GetBinLowEdge(bin),
                      histogram->GetMinimum(),
                      histogram->GetBinWidth(bin)+histogram->GetBinLowEdge(bin),
                      histogram->GetBinContent(bin));
   b->SetFillColor(color);
   b->Draw("same");

}


void patamar() {

    //INICIALIZACOES
    TApplication App("A", nullptr, nullptr);
    TCanvas C("C","",1200,800);
    TH1D* histogram = new TH1D("histogram", "Uniformly Distributed Bins", 1024, 0, 1023);
    
    std::vector<double> xData;
    std::vector<double> yData;

    //Leitura do Ficheiro
    std::ifstream file("cesio_sem_ruido.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << std::endl;
    }
    
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
        yData.push_back(cont);
        }
    }
   
    file.close();

    //HISTOGRAM

    //Adding points to graph
    for (int i=0; i<xData.size(); i++) {
        histogram->Fill(xData[i], yData[i]);
    }

    histogram->SetTitle("Pico de Absorcao Total Cesio"); // -> MEXER AQUI
    histogram->SetMarkerColor(kBlue-2);
    histogram->SetMarkerStyle(20);
    histogram->GetXaxis()->SetTitle("Bin");
    histogram->GetYaxis()->SetTitle("N de Contagens");
    histogram->SetFillColor(kRed);
    histogram->SetFillStyle(0, 330);
    histogram->Draw("hist");
        

    //BINS DO PICO -> MEXER AQUI
    double min = 0;
    double max = 500;
    histogram->GetXaxis()->SetRangeUser(min, max);

    // Criamos uma instância "fitadora"
    TF1 *fitFunc = new TF1("fitFunc", predefinedGaussian, min, max, 3);

    // Parâmetros Iniciais Estimados -> MEXER AQUI
    double amplitude = 300;
    double mean = 600;
    double stddev = 17;

    // Vai dar os parâmtros à nossa função de fit
    fitFunc->SetParameters(amplitude, mean, stddev);

    // Fit em que se ignoram os bins sem nada
    histogram->Fit(fitFunc, "W");

    double fittedAmplitude = fitFunc->GetParameter(0);
    double fittedMean = fitFunc->GetParameter(1);
    double fittedStdDev = fitFunc->GetParameter(2);

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


    //fitFunc->Draw("same");
    C.SaveAs("COMPTON.png"); // -> MEXER AQUI
    C.Update();
    C.WaitPrimitive();


    std::ofstream outputFile("Parâmetros_Pico_Absorção_Total_Césio.txt"); // -> MEXER AQUI
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

    std::cout << "File writing completed successfully." << std::endl;

  
}



