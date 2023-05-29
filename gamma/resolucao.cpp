#include <TGraphErrors.h>
#include <TF1.h>
#include <cmath>
#include <TCanvas.h>
#include <TMath.h>

Double_t fitFunction(Double_t* x, Double_t* par) {
    // Fit function definition
    // par[0] corresponds to the fit parameter(s)
    // Return the predicted y-value for a given x-value
    // based on the fit parameters
    // Example: par[0] * x[0] + par[1] * x[0] * x[0]
    // Replace with your actual fit function
    return par[0]/sqrt(x[0]+par[1]) + par[2];
}

void fitData() {
    // Step 1: Set up your data points
    Double_t x[] = {662, 32, 1173, 1333, 607.7, 605.7, 606.1, 30.6, 85.9, 306.9, 362.6};
    Double_t y[] = {0.06245, 0.2271, 0.05270, 0.04685, 0.05997, 0.06118, 0.06232, 0.1862, 0.0983, 0.1033, 0.09544};
    Double_t ex[] = {0, 0, 0, 0, 0.2, 0.3, 0.3, 0.1, 0.1, 0.1, 0.1};
    Double_t ey[] = {0.00001, 0.0001, 0.00001, 0.00001, 0.00004, 0.00004, 0.00004, 0.0004, 0.0001, 0.0001, 0.00006};
    Int_t nPoints = 11;

    // Step 2: Create a ROOT graph
    TGraphErrors* graph = new TGraphErrors(nPoints, x, y, ex, ey);

    // Step 3: Define the fit function
    TF1* fitFunc = new TF1("fitFunc", fitFunction, 0, 10000, 3);

    // Step 4: Set initial parameter values
    fitFunc->SetParameter(0, 1.145); // Initial value for par[0]
    fitFunc->SetParameter(1, 1.34); // Initial value for par[1]
    fitFunc->SetParameter(2, -0.01756); // Initial value for par[2]

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

    graph -> SetTitle("Energy Resolution;Energy;Resolution"); //Title, X title, Y title
    graph->Draw("AP");
    canvas -> SetLogy();
    canvas->SaveAs("resolucao.png");
    
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


int main() {
    fitData();
    return 0;
}
