Double_t fitFunction(Double_t* x, Double_t* par) {
    // Fit function definition
    // par[0] corresponds to the fit parameter(s)
    // Return the predicted y-value for a given x-value
    // based on the fit parameters
    // Example: par[0] * x[0] + par[1] * x[0] * x[0]
    // Replace with your actual fit function
    return par[0]*(1-1/sqrt(1+(0.015875/(x[0]+par[1]))*(0.015875/(x[0]+par[1]))));
}

void dist() {
    // Step 1: Set up your data points
    //Falta aqui um dos pontos, o de 80 keV
    Double_t x[] = {0.0054,0.0144,0.0237,0.0339,0.0441,0.0548};
    Double_t y[] = {1323,681,387,240,168,126};
    Double_t ex[] = {0.0001,0.0001,0.0001,0.0001,0.0001,0.0001};
    Double_t ey[] = {182,49,17,7,3,2};
    Int_t nPoints = 6;

    // Step 2: Create a ROOT graph
    TGraphErrors* graph = new TGraphErrors(nPoints, x, y, ex, ey);

    // Step 3: Define the fit function
    TF1* fitFunc = new TF1("fitFunc", fitFunction, 0, 10000, 2); //este ultimo é o numero de parametros

    // Step 4: Set initial parameter values
    fitFunc->SetParameter(0, 2600); // Initial value for par[0]
    //fitFunc->SetParameter(1, 0.03175); // Initial value for par[1]
    fitFunc->SetParameter(1, 0); // Initial value for par[2]
    //fitFunc->SetParameter(2, 1);

    // Step 5: Perform the fit
    graph->Fit(fitFunc, "QSE");

    // Step 6: Access the fit results
    Double_t chi2 = fitFunc->GetChisquare();
    double chi2Ndf = chi2 / (nPoints-2);


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

    graph -> SetTitle("Influencia da distancia na taxa de contagens;distancia [m] ;taxa de contagens [1/s]"); //Title, X title, Y title
    graph->Draw("AP");
    canvas->SaveAs("dist_exp2_bfixo.png");
    
    // Display the fit parameters
    std::cout << "Fit Parameters:" << std::endl;
    for (Int_t i = 0; i < fitFunc->GetNpar(); ++i) {
        Double_t parameterValue = fitFunc->GetParameter(i);
        std::cout << "Parameter " << i << ": " << parameterValue << " Error" << ": " <<  fitFunc->GetParError(i) <<  std::endl;
    }

    // Display the chi-square value
    std::cout << "Chi-square: " << chi2 << std::endl;
    std::cout << "Chi-square/ndf: " << chi2Ndf << std::endl;
    std::cout << "teste" << graph->GetErrorX(0) << std::endl;
}

