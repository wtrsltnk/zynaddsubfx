/*
  ZynAddSubFX - a software synthesizer

  synthdata.cpp - Wrapper class for zynaddsubfx for connecting signals to ui
  Copyright (C) 2014 Wouter Saaltink
  Author: Wouter Saaltink

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
#include "synthdata.h"
#include "../Misc/Part.h"
#include "../Misc/Master.h"
#include "../Sequence/sequence.h"

SynthData::SynthData(QObject *parent) :
    QObject(parent)
{ }

SynthData& SynthData::Instance()
{
    static SynthData data;
    return data;
}

SynthData::~SynthData()
{ }

int SynthData::GetPartCount() const { return NUM_MIDI_PARTS; }

Part* SynthData::AddPart()
{
    return 0;
}

Part* SynthData::GetPartAt(int index)
{
    if (index >= 0 && index < NUM_MIDI_PARTS)
        return Master::getInstance().part[index];
    return 0;
}

void SynthData::EnablePartAt(int index, bool enable)
{
    Master::getInstance().partonoff(index, (enable ? 1 : 0));
}

int SynthData::IndexOfPart(Part* part)
{
    for (int i = 0; i < this->GetPartCount(); i++)
        if (Master::getInstance().part[i] == part)
            return i;
    return -1;
}

void SynthData::SelectPartAt(int index, bool add)
{
    if (add == false)
        this->_selectedParts.clear();
    this->_selectedParts << index;
    this->PartSelectionChanged(this->_selectedParts);
}

void SynthData::RemovePartAt(int index)
{
    if (index >= 0 && index < NUM_MIDI_PARTS)
    {
        Master::getInstance().partonoff(index, 0);
        this->PartsChanged();
    }
}

int SynthData::GetClipCount() const { return NUM_MAX_CLIPS; }

MidiClip* SynthData::AddClip()
{
    return 0;
}

MidiClip* SynthData::GetClipAt(int index)
{
    if (index >= 0 && index < NUM_MAX_CLIPS)
        return Sequence::getInstance().Pclips[index];
    return 0;
}

int SynthData::IndexOfClip(MidiClip* clip)
{
    for (int i = 0; i < this->GetClipCount(); i++)
        if (Sequence::getInstance().Pclips[i] == clip)
            return i;
    return -1;
}

void SynthData::SelectClipAt(int index, bool add)
{
    if (add == false)
        this->_selectedClips.clear();
    this->_selectedClips << index;
    this->ClipSelectionChanged(this->_selectedClips);
}

void SynthData::RemoveClipAt(int index)
{
    if (index >= 0 && index < NUM_MAX_CLIPS)
    {
        delete Sequence::getInstance().Pclips[index];
        Sequence::getInstance().Pclips[index] = 0;
        this->ClipsChanged();
    }
}

int SynthData::GetSysFxCount() const
{
    return NUM_SYS_EFX;
}

EffectMgr* SynthData::GetSysFxAt(int index)
{
    if (index >= 0 && index < NUM_SYS_EFX)
        return Master::getInstance().sysefx[index];
    return 0;
}

void SynthData::SetVolume(int volume)
{
    Master::getInstance().setPvolume(volume);
    this->VolumeChanged(Master::getInstance().Pvolume);
}

unsigned char SynthData::GetVolume() const
{
    return Master::getInstance().Pvolume;
}

vuData SynthData::GetVuData()
{
    return Master::getInstance().getVuData();
}
