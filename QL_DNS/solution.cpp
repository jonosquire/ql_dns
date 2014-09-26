//
//  solution.cpp
//  QL_DNS
//
//  Created by Jonathan Squire on 8/10/14.
//  Copyright (c) 2014 J Squire. All rights reserved.
//

#include "solution.h"


solution::solution(Model* eqs) :
nMF_(eqs->num_MFs()), nLin_(eqs->num_Lin()), nz_(eqs->NZ()), nz_full_(eqs->NZfull()), nxy_(eqs->Dimxy())
{
    // Constructor for the solution class, contains data required for holding the QL solution
    // ONLY STORES NON-DEALIASED VALUES OF SOLUTION IN FOURIER SPACE (eqs->NZ() returns this)
    // IN kz STORED AS: (0 1 2 3 ... (NZ()-1)/2 -(NZ()-1)/2  -(NZ()-1)/2+1 ... -2 -1)
    
    linear_field_ = new dcmplxVec *[nxy_];
    for (int i=0; i<nxy_; ++i) {
        linear_field_[i] = new dcmplxVec[nLin_];
        for (int j=0; j<nLin_; ++j) {
            linear_field_[i][j] = dcmplxVec(nz_);
        }
    }
    
    mean_field_ = new dcmplxVec[nMF_];
    for (int i=0; i<nMF_; ++i) {
        mean_field_[i] = dcmplxVec(nz_);
    }
    
}

solution::~solution() {
    for (int i=0; i<nxy_; ++i) {
        delete[] linear_field_[i];
    }
    delete[] linear_field_;
    delete[] mean_field_;
}


// Initial conditions
// Only used for simple cases (e.g., zero in the fluctuations). For more complex cases, save in hdf5 in Matlab and set start_from_saved_Q to 1
void solution::Initial_Conditions(Inputs &SP,fftwPlans &fft) {
    /////////////////////////////////////////////
    //  LINEAR FIELDS
    for (int i=0; i<nxy_; ++i) {
        for (int j=0; j<nLin_; ++j) {
            linear_field_[i][j].setConstant(0.0);
        }
        
    }
    
    ////////////////////////////////////
    //   MEAN FIELDS
    // Assign to mean fields
    dcmplxVec *meanf_r = new dcmplxVec[nMF_]; // Real space version
    for (int i=0; i<nMF_; ++i) {
        meanf_r[i] = dcmplxVec(nz_full_);
    }
    doubVec zg = doubVec::LinSpaced( nz_full_, 0, 2*PI*(1.0-1.0/nz_full_) );
    
    // Decide what MF initial conditions based on SP.initial_By
    // If initial_By>0, only By nonzero, set to lowest kz mode in box
    double mult_fac;
    if (SP.initial_By > 0.0) { // Lowest Kz mode in the box, amplitude from SP.initial_By
        for (int i=0; i<nMF_; ++i) {
            if (i==1) { // Amplitude specified here, start By 10* larger than other(s)
                mult_fac = SP.initial_By;
                for (int k=0; k<nz_full_; ++k) {
                    meanf_r[i](k) = (dcmplx) mult_fac*cos( 2*zg(k) );
                }
            } else {
                mult_fac = -0.1*SP.initial_By;
                for (int k=0; k<nz_full_; ++k) {
                    meanf_r[i](k) = (dcmplx) mult_fac*cos( 2*zg(k) );
                }
            }
            
        }
    } else {// If initial_By<0, all MFs nonzero, random with specified amplitude
        for (int i=0; i<nMF_; ++i) {
            
            if (i==1) { // Amplitude specified here, start By 10* larger than other(s)
                mult_fac = -SP.initial_By;
            } else {
                mult_fac = -0.1*SP.initial_By;
            }
            meanf_r[i].real().setRandom();
            meanf_r[i].imag().setZero();
            meanf_r[i] = meanf_r[i]-meanf_r[i].mean();
            meanf_r[i] *= mult_fac;
        }
        
    }
    
    // Take the Fourier transform
    for (int i=0; i<nMF_; ++i) {
        fft.forward(meanf_r+i, mean_field_+i);
    }
    

    delete[] meanf_r;

}





