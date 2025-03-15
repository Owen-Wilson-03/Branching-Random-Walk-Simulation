#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <chrono>

using namespace std;

// Random number generator
random_device rd;
mt19937 gen(rd());
normal_distribution<> gauss(0.0, 1.0); // Standard normal distribution

// Function to update progress bar
void printProgressBar(int progress, int total) {
    int barWidth = 50;
    float percent = (float)progress / total;
    int pos = barWidth * percent;

    cout << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) cout << "#";
        else cout << "-";
    }
    cout << "] " << (int)(percent * 100.0) << "%\r" << flush;
}

// Recursively compute leaf weights and track the maximum
void computeLeafWeights(int depth, double sum, double &M_n, int &leaf_count, 
                        const vector<int>& sample_indices, int& printed, bool print_enabled) {
    if (depth == 0) {
        if (print_enabled && printed < sample_indices.size() && leaf_count == sample_indices[printed]) {
            cout << "Leaf " << sample_indices[printed] << ": " << sum << endl;
            printed++;
        }
        M_n = max(M_n, sum);
        leaf_count++;
        return;
    }
    double w1 = gauss(gen);
    double w2 = gauss(gen);
    computeLeafWeights(depth - 1, sum + w1, M_n, leaf_count, sample_indices, printed, print_enabled);
    computeLeafWeights(depth - 1, sum + w2, M_n, leaf_count, sample_indices, printed, print_enabled);
}

int main() {
    int num_trees = 100;
    int max_samples = 10; // Number of leaf weights to sample

    for (int n = 2; n <= 100; ++n) {
        vector<double> M_values;
        double sum = 0.0, sum_sq = 0.0;
        int total_leaves = (1 << n); // 2^n leaves

        cout << "\nComputing for n = " << n << "...\n";
        auto start_time = chrono::high_resolution_clock::now();

        for (int i = 0; i < num_trees; ++i) {
            double M_n = -1e9;
            int leaf_count = 0;
            int printed = 0;
            bool print_enabled = (i == 0); // Only print for one tree

            // Compute evenly spaced leaf indices
            vector<int> sample_indices;
            for (int j = 0; j < max_samples; ++j) {
                sample_indices.push_back((j * total_leaves) / max_samples);
            }

            computeLeafWeights(n, 0.0, M_n, leaf_count, sample_indices, printed, print_enabled);
            M_values.push_back(M_n);
            sum += M_n;
            sum_sq += M_n * M_n;

            if (i % (num_trees / 100 + 1) == 0) {
                printProgressBar(i + 1, num_trees);
            }
        }
        printProgressBar(num_trees, num_trees);
        cout << endl;

        // Compute mean and variance
        double mean = sum / num_trees;
        double mean_sq = sum_sq / num_trees;
        double variance = mean_sq - (mean * mean);

        cout << "n = " << n 
             << ", E[M_n] = " << mean 
             << ", Var(M_n) = " << variance 
             << endl;

        auto end_time = chrono::high_resolution_clock::now();
        double elapsed_time = chrono::duration_cast<chrono::seconds>(end_time - start_time).count();
        cout << "Time taken: " << elapsed_time << "s\n";
    }

    return 0;
}
