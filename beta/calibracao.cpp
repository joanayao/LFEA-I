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
    Double_t x[] = {113.02,243.51,44.18,446.99,328.01,658.48,547.90,875.50,175.57};
    Double_t y[] = {662,1400,255,2546,1910,3819,3183,5092,1018};
    Double_t ex[] = {0.023,0.023,0.039,0.295,0.023,0.023,0.023,0.022,0.024};
    Double_t ey[] = {25,40,18,62,49,86,74,111,32};
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

    // Step 6: Access the fit results
    Double_t chi2 = fitFunc->GetChisquare();
    double ndf = fitFunc->GetNDF();

    // Perform any additional analysis or output the results as needed
    // Save the graph as a PNG file
    TCanvas* canvas = new TCanvas("canvas");
    //Changing the apperance of error bars
    graph-> SetLineWidth(1);
    graph->SetLineColor(kBlue);

    //Changing the marker apperande
    
    graph->SetMarkerStyle(kOpenCircle);
    graph->SetMarkerSize(0.2);
    //Changing the titles and drawing

    graph -> SetTitle("Calibracao;Bins;Energy(MeV)"); //Title, X title, Y title
    graph->Draw("AP");
    canvas->SaveAs("calibracao_am.png");
    
    // Display the fit parameters
    std::cout << "Fit Parameters:" << std::endl;
    for (Int_t i = 0; i < fitFunc->GetNpar(); ++i) {
        Double_t parameterValue = fitFunc->GetParameter(i);
        std::cout << "Parameter " << i << ": " << parameterValue << " Error" << ": " <<  fitFunc->GetParError(i) <<  std::endl;
    }

    // Display the chi-square value
    std::cout <<   "Chi-square: " << chi2 << std::endl;
    std::cout <<   "NDF: " << ndf << std::endl;
    std::cout <<   "chi/NDF: " << chi2/ndf << std::endl;
    
}

