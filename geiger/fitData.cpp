Double_t fitFunction(Double_t* x, Double_t* par) {
    // Fit function definition
    // par[0] corresponds to the fit parameter(s)
    // Return the predicted y-value for a given x-value
    // based on the fit parameters
    // Example: par[0] * x[0] + par[1] * x[0] * x[0]
    // Replace with your actual fit function
    return par[0]*x[0]+par[1];
}

void fitData() {
    // Step 1: Set up your data points
    //Falta aqui um dos pontos, o de 80 keV
    Double_t x[] = {800,825,850,875,900,925,950,975,1000,1025,1050,1075,1100,1125,1150,1175,1200};
    Double_t y[] = {21.2,21.1,22.2,22.7,22.8,23.8,23.4,23.5,24.6,23.8,24.6,24.1,24.7,24.9,25.3,25.4,25.9};
    Double_t ex[] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    Double_t ey[] = {0.7,0.7,0.7,0.7,0.7,0.7,0.7,0.7,0.8,0.7,0.8,0.8,0.8,0.8,0.8,0.8,0.8};
    Int_t nPoints = 17;

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

    graph -> SetTitle("Plateau Geiger;Voltage [V];Count Rate"); //Title, X title, Y title
    graph->Draw("AP");
    canvas->SaveAs("plateau.png");
    
    // Display the fit parameters
    std::cout << "Fit Parameters:" << std::endl;
    for (Int_t i = 0; i < fitFunc->GetNpar(); ++i) {
        Double_t parameterValue = fitFunc->GetParameter(i);
        std::cout << "Parameter " << i << ": " << parameterValue << " Error" << ": " <<  fitFunc->GetParError(i) <<  std::endl;
    }

    // Display the chi-square value
    std::cout << "Chi-square: " << chi2 << std::endl;
    std::cout << "testwe" << graph->GetErrorX(0) << std::endl;
}

