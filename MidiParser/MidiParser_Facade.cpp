# include "stdafx.h"
# include "MidiParser_Facade.h"
# include "MidiTimeCalculator.h"

#define MIDI_PARSER_FACADE(CHAR_TYPE)							\
MidiParser_Facade::MidiParser_Facade(const CHAR_TYPE* fileName)	\
{																\
	MidiTimeCalculator midiFile;								\
	midiFile.LoadMidiData(fileName);							\
	midiFile.CalcDeltaTimes();									\
																\
	times_ = midiFile.GetTimes();								\
	notes_ = midiFile.GetNotes();								\
}
MIDI_PARSER_FACADE(char)
MIDI_PARSER_FACADE(wchar_t)