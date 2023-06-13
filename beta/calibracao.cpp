Double_t fitFunction(Double_t* x, Double_t* par) {
    // Fit function definition
    // par[0] corresponds to the fit parameter(s)
    // Return the predicted y-value for a given x-value
    // based on the fit parameters
    // Example: par[0] * x[0] + par[1] * x[0] * x[0]
    // Replace with your actual fit function
    return par[0]*x[0]+par[1];
}

void calibracao() {
    // Step 1: Set up your data points
    //Falta aqui um dos pontos, o de 80 keV
    Double_t x[] = {113.52,243.99,44.83,439.24,328.50,659.01,548.38,876.01,173.08};
    Double_t y[] = {662,1400,255,2546,1910,3819,3183,5092,1018};
    Double_t ex[] = {0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02,0.02};
    Double_t ey[] = {0,30,14,51,39,75,63,99,23};
    Int_t nPoints = 9;

    // Step 2: Create a ROOT graph
    TGraphErrors* graph = new TGraphErrors(nPoints, x, y, ex, ey);

    // Step 3: Define the fit function
    TF1* fitFunc = new TF1("fitFunc", fitFunction, 0, 10000, 2);

    // Step 4: Set initial parameter values
    fitFunc->SetParameter(0, 1.145); // Initial value for par[0]
    fitFunc->SetParameter(1, 1); // Initial value for par[1]
    //fitFunc->SetParameter(2, 0.01756); // Initial value for par[2]

    // Step 5: Perform the fit
    graph->Fit(fitFunc, "QSE");

    // Perform any additional analysis or output the results as needed
    // Save the graph as a PNG file
    TCanvas* canvas = new TCanvas("canvas");
    //Changing the apperance of error bars
    graph-> SetLineWidth(1);
    graph->SetLineColor(kBlue);
    graph->GetYaxis()->SetRangeUser(0, 5800);

    //Changing the marker apperande
    
    graph->SetMarkerStyle(kOpenCircle);
    graph->SetMarkerSize(0.2);
    //Changing the titles and drawing

    graph -> SetTitle("Calibracao;Bins;Energy(KeV)"); //Title, X title, Y title
    graph->Draw("AP");
    canvas->SaveAs("calibracao_am.png");
    
    // Display the fit parameters
    std::cout << "Fit Parameters:" << std::endl;
    for (Int_t i = 0; i < fitFunc->GetNpar(); ++i) {
        Double_t parameterValue = fitFunc->GetParameter(i);
        std::cout << "Parameter " << i << ": " << parameterValue << " Error" << ": " <<  fitFunc->GetParError(i) <<  std::endl;
    }

    TFitResultPtr fitResult = graph->Fit(fitFunc, "S");
    double chi = fitResult->Chi2();
    int ndf = fitResult->Ndf();
    double chi_ndf = chi/ndf;
    cout << "chi/ndf: " << chi_ndf << endl;

    
}

