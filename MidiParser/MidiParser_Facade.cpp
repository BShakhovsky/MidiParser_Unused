# include "stdafx.h"
# include "MidiParser_Facade.h"
# include "MidiTimeCalculator.h"

MidiParser_Facade::MidiParser_Facade(const char* fileName)
{
	using namespace std;

	MidiTimeCalculator midiFile;
	midiFile.LoadMidiData(fileName);
	midiFile.CalcDeltaTimes();

	times_ = midiFile.GetTimes();
	notes_ = midiFile.GetNotes();
}