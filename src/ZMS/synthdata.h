/*
  ZynAddSubFX - a software synthesizer

  synthdata.h - Wrapper class for zynaddsubfx for connecting signals to ui
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
#ifndef SYNTHDATA_H
#define SYNTHDATA_H

#include <QObject>

class Part;
class EffectMgr;
struct vuData;
class MidiClip;

class SynthData : public QObject
{
    Q_OBJECT

    QList<int> _selectedParts;
    QList<int> _selectedClips;

    explicit SynthData(QObject *parent = 0);
public:
    static SynthData& Instance();
    virtual ~SynthData();

    int GetPartCount() const;
    Part* AddPart();
    Part* GetPartAt(int index);
    void EnablePartAt(int index, bool enable);
    int IndexOfPart(Part* part);
    void SelectPartAt(int index, bool add = false);
    void RemovePartAt(int index);

    int GetClipCount() const;
    MidiClip* AddClip();
    MidiClip* GetClipAt(int index);
    int IndexOfClip(MidiClip* clip);
    void SelectClipAt(int index, bool add = false);
    void RemoveClipAt(int index);

    int GetSysFxCount() const;
    EffectMgr* GetSysFxAt(int index);

    void SetVolume(int volume);
    unsigned char GetVolume() const;

    vuData GetVuData();
signals:
    void PartsChanged();
    void PartDataChanged(Part* part);
    void PartSelectionChanged(const QList<int>& indices);

    void ClipsChanged();
    void ClipDataChanged(MidiClip* clip);
    void ClipSelectionChanged(const QList<int>& indices);

    void VolumeChanged(unsigned char volume);

public slots:

};

#endif // SYNTHDATA_H
