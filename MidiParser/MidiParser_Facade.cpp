# include "stdafx.h"
# include "MidiParser_Facade.h"
# include "MidiTimeCalculator.h"

#define MIDI_PARSER_FACADE(CHAR_TYPE)							\
MidiParser_Facade::MidiParser_Facade(const CHAR_TYPE* fileName)	\
{																\
	using std::vector; using std::string;						\
																\
	MidiTimeCalculator midiFile;								\
	try															\
	{															\
		midiFile.LoadMidiData(fileName);						\
		midiFile.CalcDeltaTimes();								\
																\
		times_ = midiFile.GetTimes();							\
		notes_ = midiFile.GetNotes();							\
																\
		log_ = midiFile.GetLog();								\
		trackNames_ = midiFile.GetTrackNames();					\
	}															\
	catch (const std::exception&)								\
	{															\
		times_ = vector<vector<unsigned>>();					\
		notes_ = vector<vector<int16_t>>();						\
																\
		log_ = string();										\
		trackNames_ = vector<string>();							\
	}															\
}
MIDI_PARSER_FACADE(char)
MIDI_PARSER_FACADE(wchar_t)