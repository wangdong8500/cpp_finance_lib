/*
    uses source files 
    AntiThetic.cpp
    Arrays.cpp,  
    ConvergenceTable.cpp, 
    ExoticBSEngine.cpp
    ExoticEngine.cpp
    StatisticsMC.cpp
    Normals.cpp
    Parameters.cpp,
    ParkMiller.cpp,
    PathDependent.cpp
    PathDependentAsian.cpp
    PayOff.cpp
    Random2.cpp,
  */
#include <ParkMiller.h>
#include <iostream>
using namespace std;
#include <StatisticsMC.h>
#include <ConvergenceTable.h>
#include <AntiThetic.h>
#include <PathDependentAsian.h>
#include <ExoticEngineBS.h>

int main()
{

	double expiry;
	double strike; 
	double spot; 
	double vol; 
	double r;
    double d;
	unsigned long number_of_paths;
    unsigned number_of_dates;

	cout << "\nEnter expiry\n";
	cin >> expiry;


	cout << "\nStrike\n";
	cin >> strike;


	cout << "\nEnter spot\n";
	cin >> spot;

	cout << "\nEnter vol\n";
	cin >> vol;


	cout << "\nr\n";
	cin >> r;

    cout << "\nd\n";
    cin >> d;

    cout << "Number of dates\n";
    cin >> number_of_dates;

	cout << "\nNumber of paths\n";
	cin >> number_of_paths;

    CallPayOff the_pay_off(strike);


    MJArray times(number_of_dates);

    for (unsigned long i=0; i < number_of_dates; i++)
        times[i] = (i+1.0)*expiry/number_of_dates;

    ParameterConstant vol_param(vol);
    ParameterConstant r_param(r);
    ParameterConstant d_param(d);

    PathDependentAsian the_option(times, expiry, the_pay_off);

    StatisticsMean gatherer;
    ConvergenceTable convergence_gatherer(gatherer);

   
    RandomParkMiller generator(number_of_dates);
    
    Antithetic generator_antithetic(generator);

    ExoticEngineBS the_engine(r_param, d_param, vol_param, the_option, generator_antithetic, spot);

    TerminatorPath terminator_path(number_of_paths);
    TerminatorTime terminator_time(5.);
    TerminatorCombine terminator(terminator_path,terminator_time);

    the_engine.DoSimulation(convergence_gatherer, terminator);

    vector<vector<double> > results =convergence_gatherer.GetResultsSoFar();


	cout <<"\nFor the Asian call price the results are \n";
  
    {
    for (unsigned long i=0; i < results.size(); i++)
    {
        for (unsigned long j=0; j < results[i].size(); j++)
            cout << results[i][j] << " ";

        cout << "\n";
    }}

	return 0;

}
