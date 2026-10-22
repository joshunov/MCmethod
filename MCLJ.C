#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <iomanip>

using namespace std;

#define DEBUG(x) std::cout << "DEBUG: " << #x << " = " << x << " at line " << __LINE__ << std::endl


void initLattice(vector<double>& X, vector<double>& Y, double L);

double wrap(double x, double L);

double particle_energy(
    int i, double xi, double yi, const vector<double>& X, const vector<double>& Y,
    double L, double rc, double sigma, double eps);

double total_energy(const vector<double>& X, const vector<double>& Y,
     double L, double rc, double sigma, double eps);

double rand01();



int main(int argc, char* argv[]){

    if (argc != 4){
        cerr << "Usage: " << argv[0] << " <step_N> <T> <rho>" << endl;
        return 1;
    }

    int    step_N = stoi(argv[1]);   // number of MC steps
    double T = stod(argv[2]);
    double rho = stod(argv[3]); // density

    
    int particle_N = 100;
    double eps = 1;
    double sigma = 1;
    double side_len = sqrt((particle_N/rho)); // L = sqrt(volume) = sqrt(N/rho)
    double rc = 2.5 * sigma;
    double delta = 0.15*side_len; // Max displacement

    vector<double> X(particle_N,0);
    vector<double> Y(particle_N,0);

    initLattice(X, Y, side_len);

    double E = total_energy(X, Y, side_len, rc, sigma, eps);
    int Naccept = 0;

    for (int step = 0; step < step_N; step++){

        int i = rand() % particle_N; // uniform distribution to begin with.

        double E_old = particle_energy(i, X[i], Y[i], X, Y, side_len, rc, sigma, eps);


        double x_new = wrap(X[i] + delta * (2.0 * rand01() - 1.0), side_len);
        double y_new = wrap(Y[i] + delta * (2.0 * rand01() - 1.0), side_len);

        double E_new = particle_energy(i, x_new, y_new, X, Y, side_len, rc, sigma, eps);
        double dE = E_new - E_old;

        // Metropolis criterion
        if(dE <= 0 || rand01() < exp(-dE / T)){

            X[i] = x_new;
            Y[i] = y_new;

            E += dE;

            Naccept++;

            // DEBUG(Naccept);

        }

    }

    cout << "Acceptance: " << (double)Naccept / step_N << endl;

    // Check conservation of energy:
    cout << setprecision(15); 
    cout << "E/N final:  " << E / particle_N << endl;
    cout << "E/N check:  " << total_energy(X, Y, side_len, rc, sigma, eps) / particle_N << endl;

}

void initLattice(vector<double>& X, vector<double>& Y, double L){
    int N = X.size();
    int n = ceil(sqrt(N));          // sites per side
    double a = L / n;               // lattice spacing
    for (int k = 0; k < N; k++){
        X[k] = (k % n + 0.5) * a;
        Y[k] = (k / n + 0.5) * a;
    }
}

double particle_energy(int i, double xi, double yi, const vector<double>& X, const vector<double>& Y, double L, double rc, double sigma, double eps){
    double e = 0.0;

    for (int j = 0; j < (int)X.size(); j++){
        if (j == i){ // dont compare particle dist to itself
            continue;
        }

        double dx = xi - X[j];
        double dy = yi - Y[j];

        dx -= L * round(dx / L);   // minimum image
        dy -= L * round(dy / L);

        double r = sqrt(dx*dx + dy*dy);

        if (r < rc){
            e += 4.0 * eps * (pow((sigma/r),12) - pow((sigma/r),6)); // LJ Potential
            // DEBUG(e);
        }

    }

    return e;

}

double total_energy(const vector<double>& X, const vector<double>& Y, double L, double rc, double sigma, double eps){

    double E = 0.0;

    for (int i = 0; i < (int)X.size(); i++){
        E += particle_energy(i, X[i], Y[i], X, Y, L, rc, sigma, eps);
        }

    return 0.5 * E; // each pair was counted twice (i -> j) and (j -> i)
    // EASY PLACE TO OPTIMISE CODE!
    
}

double wrap(double x, double L){
    return x - L*floor(x/L);
}

double rand01(){ // Random number between [0,1]
    return (double)rand() / ((double)RAND_MAX);
}

// Optimisation points:
    // pow6 funct and pow12 = pow6 output ^2
    // dont sqrt(r), just square entire term in LJ pot calc
    // double counting of total energy points
    // dont include energy if 

