// l x l binary grid


#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
using namespace std;

// double dummy(d,T){
//     // temperature check
// }

int main(){

    float T;
    cout << "Input Temperature: " ;
    cin >> T;
    int N = 100;
    int l = 100;
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
        if (dE <= 0 || r < exp(-dE / T)) {
            grid[xi][yi] = -grid[xi][yi];   // accept
        }
        // otherwise reject
    }

        // TODO: Add equilibriation
    int M = 0;
    for (int i = 0; i < l; i++)
        for (int j = 0; j < l; j++)
            M += grid[i][j];

    cout << "m = " << (double)abs(M) / (l * l) << endl;



}

