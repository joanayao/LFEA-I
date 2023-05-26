
void cesio_fit() {

    //INICIALIZACOES
    TApplication App("A", nullptr, nullptr);
    TCanvas C("C","",1200,800);
    TH1F* histogram = new TH1F("histogram", "Uniformly Distributed Bins", 300, 0, 1023);
    
    std::vector<double> xData;
    std::vector<double> yData;

    //CALIBRACAO
    double ordenada = 8.75;
    double declive = 0.6703;

    std::ifstream file("cesio_sem_ruido.txt");
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << "COM.FONTE.txt" << std::endl;
        return;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        double bin, cont, energy;
        char comma;
        std::stringstream ss(line);
        ss >> bin >> comma >> cont;
        energy = (bin-ordenada)/declive;
        xData.push_back(energy);
        yData.push_back(cont);
    }
    
    file.close();


    /*//HISTOGRAM
    //Adding points to graph
    for (int i=0; i<xData.size(); i++) {
        histogram->Fill(xData[i], yData[i]);
    }
    histogram->SetTitle("Amplitude dos Pulsos");
        histogram->SetMarkerColor(kBlue-2);
        histogram->GetXaxis()->SetTitle("Amplitude");
        histogram->GetYaxis()->SetTitle("N de Contagens");
        histogram->Draw("HIST");
        */

    //PLOT
    TGraph* graph = new TGraph(xData.size(), xData.data(), yData.data());
    graph->Draw("AP");

    //BINS DO PICO
    double min = 420;
    double max = 500;
    double energia_min = (min-ordenada)/declive;
    double energia_max = (max-ordenada)/declive;
    graph->GetXaxis()->SetRangeUser(energia_min, energia_max);



    //FIT
    TF1 *gaussian = new TF1("gaussian", "gaus", energia_min, energia_max);
    gaussian->SetParameters(0, 1, 1);
    //histogram->Fit(gaussian);
    
    TFitResultPtr fitResult = graph->Fit(gaussian, "S");

    /*// Check if the fit converged successfully
    if (fitResult->IsValid()) {
        // Retrieve the fitted parameter values
        double fittedMean = gaussian->GetParameter(1);
        double fittedSigma = gaussian->GetParameter(2);
        double fittedAmplitude = gaussian->GetParameter(0);

        // Print the fitted parameter values
        std::cout << "Fitted Mean: " << fittedMean << std::endl;
        std::cout << "Fitted Sigma: " << fittedSigma << std::endl;
        std::cout << "Fitted Amplitude: " << fittedAmplitude << std::endl;
    } else {
        std::cout << "Fit did not converge successfully!" << std::endl;
    }
    */




    

    //TF1* fitFunc = new TF1("fitFunc", "gaus", 0, 10);
    //TFitResultPtr fitResult = histogram->Fit(fitFunc, "S");
    //fitFunc->Draw("same");
    gaussian->Draw("same");
    C.SaveAs("FIT_PICO.png");
    C.Update();
    C.WaitPrimitive();

}