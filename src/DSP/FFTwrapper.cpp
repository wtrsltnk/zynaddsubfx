/*
  ZynAddSubFX - a software synthesizer

  FFTwrapper.c  -  A wrapper for Fast Fourier Transforms
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Author: Nasca Octavian Paul

  This program is free software; you can redistribute it and/or modify
  it under the terms of version 2 of the GNU General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License (version 2 or later) for more details.

  You should have received a copy of the GNU General Public License (version 2)
  along with this program; if not, write to the Free Software Foundation,
  Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

*/

#include <cmath>
#include <cassert>
#include <cstring>
#include "FFTwrapper.h"
#include <fftw3.h>

class FFTwrapper_Pimpl
{
public:
    FFTwrapper_Pimpl(FFTwrapper* _parent) : parent(_parent) { }

    FFTwrapper* parent;
    int fftsize;
    fftw_real    *time;
    fftw_complex *fft;
    fftw_plan     planfftw, planfftw_inv;
};

FFTwrapper::FFTwrapper(int fftsize_)
    : pimpl(new FFTwrapper_Pimpl(this))
{
    pimpl->fftsize  = fftsize_;
    pimpl->time     = new fftw_real[pimpl->fftsize];
    pimpl->fft      = new fftw_complex[pimpl->fftsize + 1];
    pimpl->planfftw = fftw_plan_dft_r2c_1d(pimpl->fftsize,
                                    pimpl->time,
                                    pimpl->fft,
                                    FFTW_ESTIMATE);
    pimpl->planfftw_inv = fftw_plan_dft_c2r_1d(pimpl->fftsize,
                                        pimpl->fft,
                                        pimpl->time,
                                        FFTW_ESTIMATE);
}

FFTwrapper::~FFTwrapper()
{
    fftw_destroy_plan(pimpl->planfftw);
    fftw_destroy_plan(pimpl->planfftw_inv);

    delete [] pimpl->time;
    delete [] pimpl->fft;
}

void FFTwrapper::smps2freqs(const float *smps, fft_t *freqs)
{
    //Load data
    for(int i = 0; i < pimpl->fftsize; ++i)
        pimpl->time[i] = static_cast<double>(smps[i]);

    //DFT
    fftw_execute(pimpl->planfftw);

    //Grab data
    memcpy((void *)freqs, (const void *)pimpl->fft, pimpl->fftsize * sizeof(double));
}

void FFTwrapper::freqs2smps(const fft_t *freqs, float *smps)
{
    //Load data
    memcpy((void *)pimpl->fft, (const void *)freqs, pimpl->fftsize * sizeof(double));

    //clear unused freq channel
    pimpl->fft[pimpl->fftsize / 2][0] = 0.0f;
    pimpl->fft[pimpl->fftsize / 2][1] = 0.0f;

    //IDFT
    fftw_execute(pimpl->planfftw_inv);

    //Grab data
    for(int i = 0; i < pimpl->fftsize; ++i)
        smps[i] = static_cast<float>(pimpl->time[i]);
}

void FFT_cleanup()
{
    fftw_cleanup();
}
