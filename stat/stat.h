// "stat.h" -*- C++ -*- 

// Copyrights (C) 2021 Mattia Sotgia

#ifndef _STAT_H
#define _STAT_H

// This code is completely open-source, 
// any feature addition is welcome!

// stat library, v1.0 as of 2021-02-16

#include <vector>

class stat{
public:
    stat();

    virtual ~stat();

    void read_values(std::vector<double> &v)
    {
        clear(); // Zeroing vector before new assignment
        _v = v;
    }

    double avg(); // Compute average for input vector
    double std_err(); // Compute SE for input vector
    void clear(); // Clear input vector

private:
    std::vector<double> _v;
    double sqr_avg(); // Private compute 
    double variance(); // Private compute 
    double emp_var(); // Private compute 
    double std(); // Private compute 
    double std_adp(); // Private compute 
};

#endif
