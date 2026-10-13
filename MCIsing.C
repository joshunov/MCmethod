// l x l binary grid


#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
using namespace std;


int main(){

    float T;
    cout << "Input Temperature: " ;
    cin >> T;

    int l;
    cout << "Input grid side length: " ;
    cin >> l;

    int N;
    cout << "Input Number of Steps, N: " ;
    cin >> N;

    int n_equil = N / 2;            // discard first half
    int meas_every = l * l;         // measure once per sweep
    double m_sum = 0;
    int n_meas = 0;

    int grid[l][l] = {}; // start all spin down

    // Initialise grid to have random elements +- 1
    for (int i = 0; i < l; i++){
        for (int j = 0; j < l; j++){
            if (rand() % 2){
                grid[i][j] = 1;
            }
            else {
                grid[i][j] = -1;
            }
        }
    }

    // Loop over N and choose a random element to flip

    for (int i = 0; i < N; i++){

        int xi = rand() % l;
        int yi = rand() % l;
        int up    = grid[xi][(yi + 1) % l];
        int down  = grid[xi][(yi - 1 + l) % l];
        int left  = grid[(xi - 1 + l) % l][yi];// boolean operations required to not exceed bounds
        int right = grid[(xi + 1) % l][yi];    // 

        int dE = 2 * grid[xi][yi] * (up + down + left + right);   // Calculate deltaE

        double r = (double)rand() / RAND_MAX;
        if (dE <= 0 || r < exp(-dE / T)) {    // accept condition
            grid[xi][yi] = -grid[xi][yi];   
        }
        // otherwise reject
        

        // Calculate net magnitisation every lxl flips, only after N/2 flips
        if (i >= n_equil && i % meas_every == 0) {
            int M = 0;
            for (int a = 0; a < l; a++) {
                for (int b = 0; b < l; b++) {
                    M += grid[a][b];
                }
            }

            m_sum += (double)abs(M) / (l * l);
            n_meas++;
        }

    }
    cout << "T = " << T << ", <m> = " << m_sum / n_meas << endl;
}

