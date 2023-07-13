#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Sparse>

using namespace std;
using namespace Eigen;

double G = 6.674;

struct body {
    double M;
    Vector3d x,v,a; SparseVector<double> m;

    body(double M, Vector3d x, Vector3d v) {
        this->M = M; this->x = x; this->v = v; this->a = {0,0,0};
    }

    // funkcija nastavi masne vektorje za vsa telesa v seznamu bodies 
    static void populate_mass_vectors(vector<body>& bodies) {
        int N = bodies.size();

        // inicializirajmo sprva vse masne vektoje
        for(int i = 0; i < N; i++) {
            bodies[i].m = SparseVector<double>((N*(N-1))/2);
            bodies[i].m.reserve(N);
        }

        // nastavimo masne vektorje na vrednosti kot so opisane v 02_simulacija.md
        int k = 0;
        for(int i = 0; i < N; i++){
            for(int j = i+1; j < N; j++){
                bodies[i].m.coeffRef(j-i-1+k) = +bodies[j].M;
                bodies[j].m.coeffRef(j-i-1+k) = -bodies[i].M;
            }
            k += N-i-1;
        }
    }
};

// funckija napolni stolpce R matrike z r(i,j)/|r(i,j)|^3 in vrne najmanjši kvadrat norme
double fill_R_matrix(Matrix3Xd& R, const vector<body>& bodies) {
    int N = bodies.size(); double min_norm = __DBL_MAX__;

    int k = 0;
    for(int i = 0; i < N; i++){
        for(int j = i+1; j < N; j++){
            R.col(k) = bodies[j].x-bodies[i].x;
            double norm = R.col(k).squaredNorm(); min_norm = min(min_norm, norm);
            R.col(k) /= pow(norm, 1.5);
            k++;
        }
    }

    return min_norm;
}

// simulira gibanje teles za obdobje time, z standardnim korakom časa sdt
void simulate(vector<body>& bodies, Matrix3Xd& R, double time, double sdt = 1, int out_steps = 1000) {
    int N = bodies.size(); double pdt = time/out_steps;
    double t = 0, next_t = 0; int done_steps = 0;
    while(t <= time){
        // preverimo če je čas za prikaz koordinat
        if(t >= next_t){
             for(int j = 0; j < N; j++) cout << bodies[j].x.transpose() - bodies[bodies.size()-1].x.transpose() << endl;
             next_t = t + pdt; done_steps++;
        }

        double norm = fill_R_matrix(R, bodies);
        double dt = sdt;

        // povečamo vektoje lege in hitrosti za vsa telesa za dxj in dvj
        for(int j = 0; j < N; j++) {
            Vector3d a = G*(R*bodies[j].m);

            bodies[j].x += bodies[j].v * dt + 0.5 * bodies[j].a * dt*dt;
            bodies[j].v += 0.5 * (bodies[j].a + a) * dt;
            bodies[j].a = a;
        }
        t += dt;
    }

    // poskrbimo za kake off-by-one napake 
    for(int i = done_steps; i < out_steps; i++){
        for(int j = 0; j < N; j++) cout << bodies[j].x.transpose() - bodies[bodies.size()-1].x.transpose() << endl;
    }
}

int main() {
    int N, output_steps; double simulation_time, dt; vector<body> bodies;
    cin >> G >> dt >> simulation_time >> output_steps >> N; 
    
    for(int i = 0; i < N; i++) {
        double M; Vector3d x, v;
        cin >> M;
        cin >> x[0] >> x[1] >> x[2];
        cin >> v[0] >> v[1] >> v[2];

        bodies.push_back(body(M, x, v));
    }

    body::populate_mass_vectors(bodies);
    Matrix3Xd R(3,(N*(N-1))/2);

    simulate(bodies, R, simulation_time, dt, output_steps);
}
