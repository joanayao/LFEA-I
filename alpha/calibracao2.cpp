Double_t fitFunction(Double_t* x, Double_t* par) {
    // Fit function definition
    // par[0] corresponds to the fit parameter(s)
    // Return the predicted y-value for a given x-value
    // based on the fit parameters
    // Example: par[0] * x[0] + par[1] * x[0] * x[0]
    // Replace with your actual fit function
    return par[0]*x[0]+par[1];
}

void calibracao2() {
    // Step 1: Set up your data points
    //Falta aqui um dos pontos, o de 80 keV
    Double_t x[] = {702.87,601.61,534.91,943.59,806.7,399.82};
    Double_t y[] = {5.48556,4.71,4.19,7.33,6.28,3.14};
    Double_t ex[] = {0.06,0.09,0.09,0.11,0.11,0.11};
    Double_t ey[] = {0.00,0.05,0.05,0.07,0.06,0.04};
    Int_t nPoints = 6;

    // Step 2: Create a ROOT graph
    TGraphErrors* graph = new TGraphErrors(nPoints, x, y, ex, ey);

    // Step 3: Define the fit function
    TF1* fitFunc = new TF1("fitFunc", fitFunction, 0, 1024, 2);

    // Step 4: Set initial parameter values
    fitFunc->SetParameter(0, 1.145); // Initial value for par[0]
    fitFunc->SetParameter(1, 1); // Initial value for par[1]
    //fitFunc->SetParameter(2, 0.01756); // Initial value for par[2]

    // Step 5: Perform the fit
    graph->Fit(fitFunc, "QSE");


    // Step 6: Access the fit results
    // Save the graph as a PNG file
    Double_t chi2 = fitFunc->GetChisquare();
    TCanvas* canvas = new TCanvas("canvas");

    // Set a legend 
    TLegend* legend = new TLegend(.15, 0.75, 0.5, 0.9); // Adjust the coordinates as needed
    legend->SetTextAlign(11);
    legend->SetTextSize(0.03);
    legend->SetFillColor(0); // Transparent background
    for (Double_t i = 0; i < fitFunc->GetNpar(); ++i) {
        Double_t parameterValue = fitFunc->GetParameter(i);
        Double_t parameterError = fitFunc ->GetParError(i);
        legend->AddEntry(graph, Form("Parameter %4.f : %.4f #pm %.4f", i, parameterValue, parameterError), "");
    }
    Int_t ndf = fitFunc->GetNDF();
    legend->AddEntry(graph, Form("#chi^{2} / NDF: %.4f / %.1d", chi2, ndf), "");
    legend->SetMargin(0.1);

    // Perform any additional analysis or output the results as needed
  
    //Changing the apperance of error bars
    graph-> SetLineWidth(1);
    graph->SetLineColor(kBlue);

    //Changing the marker apperande
    
    graph->SetMarkerStyle(kOpenCircle);
    graph->SetMarkerSize(0.2);
    //Changing the titles and drawing

    graph -> SetTitle("Calibracao;Bins;Energy(MeV)"); //Title, X title, Y title
    graph -> GetXaxis() -> SetLimits(0,1024);
    graph -> GetYaxis() -> SetRangeUser(0,8);

    graph->Draw("AP");
    legend -> Draw();
    canvas->SaveAs("calibracao2_nova_am.png");
    
    // Display the fit parameters
    std::cout << "Fit Parameters:" << std::endl;
    for (Int_t i = 0; i < fitFunc->GetNpar(); ++i) {
        Double_t parameterValue = fitFunc->GetParameter(i);
        std::cout << "Parameter " << i << ": " << parameterValue << " Error" << ": " <<  fitFunc->GetParError(i) <<  std::endl;
    }
    

    // Display the chi-square value
    std::cout <<   "Chi-square: " << chi2 << std::endl;
    std::cout << "chi/ndf : " << chi2/(nPoints-fitFunc->GetNpar()) << std::endl;
}

