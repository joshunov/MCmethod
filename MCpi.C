// Use the monte-carlo method to compute pi
// using a unit square with a quarter unit circle low energy and the rest high


#include <iostream>
#include <cmath>
#include <cstdlib>
using namespace std;

int main(){
    // For a unit square, 
    int N;

    cout << "Input sample size: " ;
    cin >> N;
    int count_inside = 0;

    // For N samples, sample random point in unit square
    for (int i = 0; i < N; i++){
        float xi = (float)rand()/RAND_MAX;
        float yi = (float)rand()/RAND_MAX;

        //cout << xi << " and " << yi << endl;


        // Point is inside quarter circle if sqrt(x^2 + y^2) =< 1
        if ((xi*xi + yi*yi) <= 1.0){
            count_inside++;
        }

    }

    cout << count_inside << "/" << N << " Inside, pi estimation: " << ((float)count_inside/(float)N)*4.0 << endl;

}