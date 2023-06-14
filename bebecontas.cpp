std::vector Chn;
std::vector N_count;
std::vector N_count_e;
std::vector Energy;
std::vector Energy_e;
std::vector W;
std::vector W_e;
std::vector P;
std::vector P_e;
std::A_f;
std::A_f_e;
double calibration_slope = 1;
double slope_e = 0.1;
double calibration_point = 0;
double point_e = 0.1;


for (int i =0; i < Chn.size(); i++){
    Energy[i] = calibration_slope*Chn[i]+calibration_point;
    Energy_e[i] = sqrt(slope_E*Chn[i]*slope_e*Chn[i]+point_e*point_e);
    W[i] = Energy[i]/511+1;
    W_e[i] = Energy[i]/511;
    P[i] = sqrt(W[i]*W[i]-1);
    P_e[i] = abs(W[i]/P[i])*W_e[i];
    A_f[i] =sqrt(N_count[i]/G[P[i]])/W[i];
    A_f_e[i]= sqrt((W_e[i]*A_f[i]/W[i])*(W_e[i]*A_f[i]/W[i])+(N_count_e[i]/(2*W[i]*W[i]*G[P[i]*A_f[i]]))*(N_count_e[i]/(2*W[i]*W[i]*G[P[i]*A_f[i]]))+(A_f[i]*dG[P[i]]/2*G[P[i]])*(A_f[i]*dG[P[i]]/2*G[P[i]]));
}    

Double_t fitFunction(Double_t* x, Double_t* par) {
    // Fit function definition
    // par[0] corresponds to the fit parameter(s)
    // Return the predicted y-value for a given x-value
    // based on the fit parameters
    // Example: par[0] * x[0] + par[1] * x[0] * x[0]
    // Replace with your actual fit function
    return par[0]*(x[0]-par[1]);
}

void calibracao2() {
    // Step 1: Set up your data points
    //Falta aqui um dos pontos, o de 80 keV
    Int_t nPoints = Energy.size();

    // Step 2: Create a ROOT graph
    TGraphErrors* graph = new TGraphErrors(nPoints, Energy, A_f, Energy_e, A_f_E);

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

    graph -> SetTitle("Limite Cinemático: Kurie Plot;Energy[keV];Kurie Points"); //Title, X title, Y title
    //graph -> GetXaxis() -> SetLimits(0,1024);
    //graph -> GetYaxis() -> SetRangeUser(0,8);

    graph->Draw("AP");
    legend -> Draw();
    canvas->SaveAs("Kurie_plot");
    
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
