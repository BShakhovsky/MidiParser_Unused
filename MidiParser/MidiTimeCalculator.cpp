# include "stdafx.h"
# include "MidiTimeCalculator.h"
# include "MidiTracksCollector.h"
# include "MidiChunksReader.h"
# include "MidiStruct.h"
# include "MidiError.h"

using namespace std;
using MidiStruct::TrackEvent;

MidiTimeCalculator::MidiTimeCalculator() :
	tempoDivision_(NULL),
	microSeconds_(NULL),
	tempoSettings_(),

	tracks_(),
	milliSeconds_(),
	notes_(),

	currentTrack_(NULL),
	currentEvent_(NULL),

	log_(),
	trackNames_()
{
	milliSeconds_.emplace_back(vector<unsigned>());
	notes_.emplace_back(vector<int16_t>());
}

MidiTimeCalculator::~MidiTimeCalculator() {}

uint32_t RealMicrosec(uint32_t deltaTime, uint32_t tempoSetting, uint16_t division)
{
	assert("TIME DIVISION IS ZERO, NOT REALLY SURE WHAT IT MEANS" && division);

	if (division & 0x80'00)
		return deltaTime * static_cast<uint32_t>(TrackEvent::microSec)
			/ MidiChunksReader::SMPTE_TicksPerSec(division);
	else
		return deltaTime * tempoSetting / division;
}

#define LOAD_MIDI_DATA(CHAR_TYPE)													\
void MidiTimeCalculator::LoadMidiData(const CHAR_TYPE* fileName)					\
{																					\
	MidiTracksCollector midiData(fileName);											\
	midiData.ReadMidiFile();														\
																					\
	tempoDivision_ = midiData.GetHeaderData()->division;							\
	tracks_ = midiData.GetTracks();													\
	if (tracks_.empty()) throw MidiError("MIDI FILE DOES NOT CONTAIN ANY TRACKS");	\
																					\
	log_ = midiData.GetLog();														\
	trackNames_ = midiData.GetTrackNames();											\
}
LOAD_MIDI_DATA(char)
LOAD_MIDI_DATA(wchar_t)

void MidiTimeCalculator::CalcDeltaTimes()
{
	using boost::format;

	do
	{
		if (tempoSettings_.empty()) assert("DELTA TIME STARTED BEFORE TEMPO IS SET" && !GetEvent().deltaTime);
		else microSeconds_ += RealMicrosec(GetEvent().deltaTime, GetTempo(), tempoDivision_);

		if (-1 == GetEvent().eventChunk.status && 0x51 == GetEvent().eventChunk.metaType)	// -1 = 0xFF
		{
			tempoSettings_.insert(make_pair(microSeconds_, GetEvent().eventChunk.metaData));
			const auto
				totalSeconds(microSeconds_ / TrackEvent::microSec),
				milliSeconds(microSeconds_ % TrackEvent::microSec * 10 / TrackEvent::microSec),

				minutes(totalSeconds / TrackEvent::minute),
				seconds(totalSeconds % TrackEvent::minute);
			log_ += (format{ "Time %d:%02d:%02d   Tempo = %d Beats per Minute\n" } %
				minutes % seconds % milliSeconds %
				(TrackEvent::microSec * TrackEvent::minute / GetTempo())).str();
		}
		else if (0x0'90 == (GetEvent().eventChunk.status & 0x0'F0)	// 0xF0 is negative ==> 0x0F0 is positive
							&& GetEvent().eventChunk.velocity)		// if velocity = 0 ==> "note-off" event
		{
			milliSeconds_.back().emplace_back(microSeconds_ / 1'000);

			notes_.back().push_back(GetEvent().eventChunk.note);
		}
	} while (!EndOfTracks());
}

bool MidiTimeCalculator::EndOfTracks()
{
	auto result(false);
	if (++currentEvent_ == tracks_.at(currentTrack_).size())
	{
		if (++currentTrack_ == tracks_.size()) result = true;
		else
		{
			currentEvent_ = NULL;
			microSeconds_ = NULL;
			milliSeconds_.emplace_back(vector<unsigned>());
			notes_.emplace_back(vector<int16_t>());
		}
	}
	return result;
}

uint32_t MidiTimeCalculator::GetTempo() const
{
	auto result(tempoSettings_.upper_bound(microSeconds_));
	if (result != tempoSettings_.begin()) --result;
	return result->second;
}

TrackEvent MidiTimeCalculator::GetEvent() const
{
	return tracks_.at(currentTrack_).at(currentEvent_);
}