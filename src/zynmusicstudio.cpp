/*
  ZynAddSubFX - a software synthesizer

  main.cpp  -  Main file of the synthesizer
  Copyright (C) 2002-2005 Nasca Octavian Paul
  Copyright (C) 2014 Wouter Saaltink
  Author: Nasca Octavian Paul, Wouter Saaltink

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

#include <iostream>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <signal.h>

#ifndef WIN32
#include <unistd.h>
#include <getopt.h>
#else
#include <Windows.h>
#endif
#include <pthread.h>
#include <QApplication>
#include <QStyleFactory>
#include <QPalette>
#include "QUI/mainwindow.h"

#include "DSP/FFTwrapper.h"
#include "Misc/Master.h"
#include "Misc/Part.h"
#include "Misc/Util.h"
#include "Misc/Dump.h"
extern Dump dump;


//Nio System
#include "Nio/Nio.h"

using namespace std;

pthread_t thr4;
Master   *master;
SYNTH_T  *synth;
int       swaplr = 0; //1 for left-right swapping

int Pexitprogram = 0;     //if the UI set this to 1, the program will exit

char *instance_name = 0;

void exitprogram();

//cleanup on signaled exit
void sigterm_exit(int /*sig*/)
{
    Pexitprogram = 1;
}

/*
 * Program initialisation
 */
void initprogram(void)
{
    cerr.precision(1);
    cerr << std::fixed;
    cerr << "\nSample Rate = \t\t" << synth->samplerate << endl;
    cerr << "Sound Buffer Size = \t" << synth->buffersize << " samples" << endl;
    cerr << "Internal latency = \t" << synth->buffersize_f * 1000.0f
    / synth->samplerate_f << " ms" << endl;
    cerr << "ADsynth Oscil.Size = \t" << synth->oscilsize << " samples" << endl;


    master = &Master::getInstance();
    master->swaplr = swaplr;

    signal(SIGINT, sigterm_exit);
    signal(SIGTERM, sigterm_exit);
}

/*
 * Program exit
 */
void exitprogram()
{
    //ensure that everything has stopped with the mutex wait
    pthread_mutex_lock(&master->mutex);
    pthread_mutex_unlock(&master->mutex);

    Nio::stop();

    delete [] denormalkillbuf;
    Master::deleteInstance();
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("Fusion"));
    QPalette p = app.palette();
    p.setColor(QPalette::Window, QColor(53,53,53));
    p.setColor(QPalette::Button, QColor(53,53,53));
    p.setColor(QPalette::Highlight, QColor(142,45,197));
    p.setColor(QPalette::ButtonText, QColor(255,255,255));
    p.setColor(QPalette::WindowText, QColor(255,255,255));
    app.setPalette(p);

    synth = new SYNTH_T;
    config.init();
    dump.startnow();

    /* Get the settings from the Config*/
    synth->samplerate = config.cfg.SampleRate;
    synth->buffersize = config.cfg.SoundBufferSize;
    synth->oscilsize  = config.cfg.OscilSize;
    swaplr = config.cfg.SwapStereo;

    Nio::preferedSampleRate(synth->samplerate);

    sprng(time(0));
    synth->alias();

    //produce denormal buf
    denormalkillbuf = new float [synth->buffersize];
    for(int i = 0; i < synth->buffersize; ++i)
        denormalkillbuf[i] = float((RND - 0.5) * 1e-16);

    initprogram();

    //Run the Nio system
    if (Nio::start())
    {
        MainWindow wnd;
        wnd.show();
        app.exec();
    }

    exitprogram();
    return 0;
}
