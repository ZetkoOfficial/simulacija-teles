#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Sparse>

using namespace std;
using namespace Eigen;

struct body {
    double M;
    Vector3d x,v; SparseVector<double> m;

    body(double M, Vector3d x, Vector3d v){
        this->M = M; this->x = x; this->v = v;
    }

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

int main() {
    vector<body> bodies = {
        {1,{0,0,0},{0,0,0}},
        {2,{0,0,0},{0,0,0}},
        {3,{0,0,0},{0,0,0}},
        {4,{0,0,0},{0,0,0}},
    };

    body::populate_mass_vectors(bodies);

    for(body& b : bodies) cout << VectorXd(b.m).transpose() << endl;
}
