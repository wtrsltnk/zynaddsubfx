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
#include <signal.h>

#ifndef WIN32
#include <unistd.h>
#include <getopt.h>
#else
#include <Windows.h>
#endif

#include "ZMS/zynapplication.h"
#include "Misc/Master.h"
#include "Misc/Part.h"
#include "Misc/Util.h"
#include "Nio/Nio.h"
#include "Sequence/sequence.h"
#include "Sequence/midiclip.h"

using namespace std;

Master   *master;
Settings  *synth;

//cleanup on signaled exit
void sigterm_exit(int /*sig*/)
{ }

/*
 * Program initialisation
 */
void initprogram(void)
{
    sprng(time(0));
    synth->alias();

    //produce denormal buf
    denormalkillbuf = new float [synth->buffersize];
    for(int i = 0; i < synth->buffersize; ++i)
        denormalkillbuf[i] = float((RND - 0.5) * 1e-16);

    master = &Master::getInstance();
    master->swaplr = config.cfg.SwapStereo;

    signal(SIGINT, sigterm_exit);
    signal(SIGTERM, sigterm_exit);
}

/*
 * Program exit
 */
int exitprogram()
{
    //ensure that everything has stopped with the mutex wait
    pthread_mutex_lock(&master->mutex);
    pthread_mutex_unlock(&master->mutex);

    Nio::stop();

    delete []denormalkillbuf;
    Master::deleteInstance();

    return 0;
}

int main(int argc, char *argv[])
{
    ZynApplication app(argc, argv);

    synth = new Settings;
    config.init();

    /* Get the settings from the Config*/
    synth->samplerate = config.cfg.SampleRate;
    synth->buffersize = config.cfg.SoundBufferSize;
    synth->oscilsize  = config.cfg.OscilSize;

    initprogram();

    Master::getInstance().partonoff(1, 1);
    Master::getInstance().part[1]->Prcvchn = 1;
    Master::getInstance().part[1]->setkititemstatus(0, 1);

    MidiClip* clip1 = new MidiClip();
    clip1->Pchannel = 0;
    clip1->Pstart = 4;
    clip1->addNote(80, 64, 1.0, 1.0);
    clip1->addNote(86, 64, 2.0, 0.5);
    clip1->addNote(80, 64, 2.5, 1.0);
    clip1->Pname = "Clip 1 channel 0";
    Sequence::getInstance().Pclips[0] = clip1;

    MidiClip* clip2 = new MidiClip();
    clip2->Pchannel = 1;
    clip2->Pstart = 8;
    clip2->addNote(80, 64, 1.0, 1.0);
    clip2->addNote(86, 64, 2.0, 0.5);
    clip2->addNote(80, 64, 2.5, 1.0);
    clip2->Pname = "Clip 2 channel 1";
    Sequence::getInstance().Pclips[1] = clip2;

    Nio::preferedSampleRate(synth->samplerate);

    //Run the Nio system
    if (Nio::start())
        app.exec();

    return exitprogram();
}
