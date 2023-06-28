#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Sparse>

using namespace std;
using namespace Eigen;

const double G = 1;

struct body {
    double M;
    Vector3d x,v; SparseVector<double> m;

    body(double M, Vector3d x, Vector3d v) {
        this->M = M; this->x = x; this->v = v;
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

// simulira gibanje teles za obdobje time, z standardnim korakom časa sdt in območjem previdnosti r(i,j) < danger_dist, ki dt zmanjša za danger_factor
void simulate(vector<body>& bodies, Matrix3Xd& R, double time, double sdt = 1, double danger_dist = 10, double danger_factor=1000) {
    int N = bodies.size();
    double t = 0, next_t = 0;
    while(t < time){
        // preverimo če je čas za prikaz koordinat
        if(t > next_t){
             for(int j = 0; j < N; j++) cout << bodies[j].x.transpose() << endl;
             next_t = t + sdt;
        }

        double norm = fill_R_matrix(R, bodies);
        double dt = sdt;

        // če je najbližja razdaja premajhna potem smo v nevarnem območju in zmanjšamo dt
        if(norm <= danger_dist)  dt /= danger_factor;

        // povečamo vektoje lege in hitrosti za vsa telesa za dxj in dvj
        for(int j = 0; j < N; j++) {
            bodies[j].x += bodies[j].v * dt;
            bodies[j].v += G * (R*bodies[j].m) * dt;
        }
        t += dt;
    }
}

int main() {
    vector<body> bodies = {
        {1,{-10,0,0},{0,0.05,0.005}},
        {1,{10,0,0},{0,-0.05,0}},
    };

    int N = bodies.size();

    body::populate_mass_vectors(bodies);
    Matrix3Xd R(3,(N*(N-1))/2);

    simulate(bodies, R, 1000, 0.5, 50);
}
