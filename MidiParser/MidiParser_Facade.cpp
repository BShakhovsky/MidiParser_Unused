# include "stdafx.h"
# include "MidiParser_Facade.h"
# include "MidiTimeCalculator.h"

using namespace std;
using namespace Model::MidiParser;

MidiParser_Facade::MidiParser_Facade(const char* fileName)
{
	MidiTimeCalculator midiFile;
	midiFile.LoadMidiData(fileName);
	midiFile.CalcDeltaTimes();

	times_ = midiFile.GetTimes();
	notes_ = midiFile.GetNotes();
}