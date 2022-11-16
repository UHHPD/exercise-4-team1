#include <iostream>
#include <cmath>
#include <random>
#include <vector>

using namespace std;

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
    // Fill me in question 1.d!

    Vec result;
    result.x = 0; // Zeroes are boring, better use some random values...
    result.y = 0;
    result.z = 0;
    return result;
}

int main()
{
    // Short demo of how to use the "Vec" type defined above. You can delete or
    // comment out this code if you wish.
    Vec something; // Just like one would do "double x;"
    something.x = 1;
    something.y = -1;
    something.z = 0;

    // We can print it
    cout << "something   = " << something << endl;

    // Or do something useful with the components
    cout << "|something| = "
         << sqrt(pow(something.x, 2) + pow(something.y, 2) + pow(something.z, 2))
         << endl;

    // Another short demo for the "Histogram" type defined above.
    Histogram counts(0, 1, 10); // 10 intervals between 0 and 1

    // Add some values to it
    counts.fill(0.05); // First bin
    counts.fill(0.06); // First bin too
    counts.fill(0.3);  // Third bin
    counts.fill(42);   // Out of range, will be ignored

    // Print it to the terminal
    cout << "Counts:" << endl;
    counts.write(cout); // One can use an ofstream instead of cout to write to a file

    return 0;
}
