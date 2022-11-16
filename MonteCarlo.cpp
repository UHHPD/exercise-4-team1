#include <iostream>
#include <fstream>
#include <cmath>
#include <random>
#include <vector>
#include <algorithm>

using namespace std; // bruh

/**
 * A very simple vector type. It merely stores the x, y, and z components.
 */
struct Vec {
    double x, y, z;
};

/**
 * This makes sure `cout << vec` does something sensible. Change at your own risk.
 */
ostream &operator<<(ostream &out, Vec v)
{
    return out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

/**
 * A barebones histogram type.
 */
class Histogram {
    double m_min, m_bin_width;
    std::vector<int> m_counts;
public:
    /// Creates an histogram with `count` bins between `min` and `max`.
    Histogram(double min, double max, int count)
    {
        m_min = min;
        m_bin_width = (max - min) / count;
        m_counts = std::vector<int>(count);
    }

    /// Adds a value to the correct bin in the histogram.
    void fill(double value)
    {
        int bin = (int) ((value - m_min) / m_bin_width);
        if (bin >= 0 && bin < m_counts.size()) {
            m_counts[bin]++;
        }
    }

    size_t counts() const {
        return std::accumulate(m_counts.begin(), m_counts.end(), 0.);
    }

    /// Writes the contents of the histogram to `out`.
    void write(ostream &out) const
    {
        for (int bin = 0; bin < m_counts.size(); ++bin) {
            out << (m_min + bin * m_bin_width) << "\t"
                << (m_min + (bin + 1) * m_bin_width) << "\t"
                << m_counts[bin] << endl;
        }
    }
};

/**
 * This function takes a mt19937 random number generator and generates a random
 * vector with `|vector| = norm`. The vector is uniformly distributed on the
 * surface of the sphere with radius `norm`.
 */
Vec randomVector(mt19937 &generator, double norm)
{
    // Init uniform distributions
    uniform_real_distribution<double> phi_distribution (0, 2*M_PI);
    uniform_real_distribution<double> ctheta_distribution (-1, 1);

    auto phi = phi_distribution(generator);
    auto ctheta = ctheta_distribution(generator);

    Vec result;
    result.x = norm * cos(phi) * sin(acos(ctheta));
    result.y = norm * sin(phi) * sin(acos(ctheta));
    result.z = norm * ctheta;
    return result;
}

double pT(Vec& vec) {
    return sqrt(pow(vec.x, 2) + pow(vec.y, 2));
}

double absCosTheta(Vec& vec, double norm = 0.) {
    if (norm <= 0.) {
        norm = sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
    }
    return abs(vec.z / norm);
}

int main()
{
    // RNG devic for random seed
    random_device rng_device {};
    // Init RNG w/ random seed else every time same random numbers
    mt19937 generator {rng_device()};
    // Init uniform distribution
    uniform_real_distribution<double> distribution (0, 1);

    /* A1
    // Lambda to draw random number
    auto gen_from_distr = [&]() { return distribution(generator); };

    // Print 5 random numbers
    std::cout << "5 random numbers:\n";
    for(size_t n = 0; n < 5; ++n) {
        std::cout << gen_from_distr() << " ";
    }
    std::cout << std::endl;

    // Fill 1000 random numbers in a histogram w/ 10 bins
    std::cout << "1000 random numbers:\n";
    Histogram hist {0., 1., 10};
    for(size_t n = 0; n < 1000; ++n) {
        hist.fill(gen_from_distr());
    }
    hist.write(std::cout);

    // 1000 vectors
    std::cout << "1000 random vectors:\n";
    Histogram hist_pT {0., 50., 50};
    for(size_t n = 0; n < 1000; ++n) {
        auto vec = randomVector(generator, 50);
        hist_pT.fill(pT(vec));
    }
    std::ofstream fout("pT.txt");
    hist_pT.write(fout);
    fout.close();
    std::cout << "written to pT.txt" << std::endl;
    */

    // A2
    // Init normal distribution for detector resolution
    normal_distribution<double> det_distribution (2000, 200);

    // 1000 measurements
    std::cout << "det: 1000 measurements:\n";
    Histogram hist_det {1000, 3000, 40};
    for (size_t n = 0; n < 1000; ++n) {
        hist_det.fill(det_distribution(generator));
    }
    std::ofstream fout_det("det.txt");
    hist_det.write(fout_det);
    fout_det.close();
    std::cout << "written to det.txt" << std::endl;

    // 1000 measurements with cuts
    std::cout << "det: 1000 measurements w/ det eff:\n";
    Histogram hist_det_deff {500, 1500, 40};
    for (size_t n = 0; n < 1000; ++n) {
        auto p = 0.5 * det_distribution(generator);
        auto vec = randomVector(generator, p);
        auto pT_ = pT(vec);
        if (!(pT_ < 500 || absCosTheta(vec, p) > 0.98)) {
            hist_det_deff.fill(pT_);
        }
    }
    std::ofstream fout_det_deff("det_deff.txt");
    hist_det_deff.write(fout_det_deff);
    fout_det_deff.close();
    std::cout << "written to det_deff.txt\n";
    std::cout << "detection efficiency: " << hist_det_deff.counts() / 1000. << std::endl;

    return 0;
}
