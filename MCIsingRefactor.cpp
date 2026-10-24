#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
using namespace std;

class Particle{
    public:

        void set_pos(double xi, double yi);
        void set_neighbours(Particle* U, Particle* D, Particle* L, Particle* R);
        void flip();

        int get_spin() const;
        int calc_dE() const; // energy change if this spin were flipped


    private:

        double x;
        double y;

        int spin = -1; // Start spin down

        Particle* above;
        Particle* below;
        Particle* left;
        Particle* right;


};

void Particle::set_pos(double xi, double yi){
    x = xi;
    y = yi;
}

void Particle::flip(){
    spin = spin * (-1);
}

int Particle::get_spin() const{
    return spin;
}

int Particle::calc_dE() const{
    int neighbour_sum = above->spin + below->spin + left->spin + right->spin;
    return 2 * spin * neighbour_sum;
}

void Particle::set_neighbours(Particle* U, Particle* D, Particle* L, Particle* R){
    above = U;
    below = D;
    left = L;
    right = R;
}

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

    vector<Particle> grid(l*l);

    // Initialise grid to have random elements +- 1
    for (int i = 0; i < l; i++){
        for (int j = 0; j < l; j++){
            grid[i*l + j].set_pos(i,j);

            if (rand() > RAND_MAX/2){
                grid[i*l + j].flip();
            }

        }
    }

    // Set neighbours for all particles

    for (int i = 0; i < l*l; i++){

        int row = i / l;
        int col = i % l;

        // Impose periodic boundary conditions here (+ l keeps the modulo non-negative)
        int u_indx = ((row - 1 + l) % l) * l + col;
        int d_indx = ((row + 1) % l) * l + col;
        int l_indx = row * l + (col - 1 + l) % l;
        int r_indx = row * l + (col + 1) % l;

        grid[i].set_neighbours(
            &grid[u_indx],
            &grid[d_indx],
            &grid[l_indx],
            &grid[r_indx]
        );
    }

    // Monte-Carlo Loop

    for (int i = 0; i < N; i++){

        // Pick a random particle to flip
        Particle& p = grid[rand() % (l*l)];

        int dE = p.calc_dE();

        double r = (double)rand() / RAND_MAX;
        if (dE <= 0 || r < exp(-dE / T)) {    // accept condition
            p.flip();
        }
        // otherwise reject

        // Calculate net magnetisation every lxl flips, only after N/2 flips
        if (i >= n_equil && i % meas_every == 0) {
            int M = 0;
            for (int a = 0; a < l*l; a++) {
                M += grid[a].get_spin();
            }

            m_sum += (double)abs(M) / (l * l);
            n_meas++;
        }

    }

    cout << "T = " << T << ", <m> = " << m_sum / n_meas << endl;

}
