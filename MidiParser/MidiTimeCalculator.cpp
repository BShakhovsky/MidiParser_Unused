# include "stdafx.h"
# include "MidiTimeCalculator.h"
# include "MidiTracksCollector.h"
# include "MidiChunksReader.h"
# include "MidiStruct.h"
# include "MidiError.h"

using namespace std;
using MidiStruct::TrackEvent;

MidiTimeCalculator::MidiTimeCalculator() :
	tempoDivision_(0ui16),
	microSeconds_(0.0l),
	tempoSettings_(),
	globalTempoSettings_(),

	tracks_(),
	milliSeconds_(),
	notes_(),

	currentTrack_(0u),
	currentEvent_(0u),

	log_(),
	trackNames_()
{
	milliSeconds_.emplace_back(vector<unsigned>());
	notes_.emplace_back(vector<int16_t>());
}

MidiTimeCalculator::~MidiTimeCalculator() {}

#define LOAD_MIDI_DATA(CHAR_TYPE)														\
void MidiTimeCalculator::LoadMidiData(const CHAR_TYPE* fileName)						\
{																						\
	MidiTracksCollector midiData(fileName);												\
	midiData.ReadMidiFile();															\
																						\
	tempoDivision_ = midiData.GetHeaderData()->division;								\
	assert("TIME DIVISION IS ZERO, NOT REALLY SURE WHAT IT MEANS" && tempoDivision_);	\
	tracks_ = midiData.GetTracks();														\
	if (tracks_.empty()) throw MidiError("MIDI FILE DOES NOT CONTAIN ANY TRACKS");		\
																						\
	log_ = midiData.GetLog();															\
	trackNames_ = midiData.GetTrackNames();												\
}
LOAD_MIDI_DATA(char)
LOAD_MIDI_DATA(wchar_t)

double MidiTimeCalculator::DeltaToMicrosec(const double deltaTime, const uint32_t tempoSetting) const
{
	return (tempoDivision_ & 0x80'00)
		? deltaTime * TrackEvent::microSec / MidiChunksReader::SMPTE_TicksPerSec(tempoDivision_)
		: deltaTime * tempoSetting / tempoDivision_;
}
double MidiTimeCalculator::MicrosecToDelta(const double microSec, const uint32_t tempoSetting) const
{
	return (tempoDivision_ & 0x80'00)
		? microSec * MidiChunksReader::SMPTE_TicksPerSec(tempoDivision_) / TrackEvent::microSec
		: microSec * tempoDivision_ / tempoSetting;
}

void MidiTimeCalculator::CalcDeltaTimes()
{
	do
	{
		ProgressMicroseconds();
		ReadEvent();
	} while (!EndOfTracks());
}

void MidiTimeCalculator::ProgressMicroseconds()
{
	if (tempoSettings_.empty())
	{
		if (GetEvent().deltaTime)	// In this case default tempo assumed 120 BPM:
			tempoSettings_.insert(make_pair(0, TrackEvent::microSec * TrackEvent::minute / 120));
									// Event meta data = 1'000'000 * 60 / 120 = 500'000
	}
	else
	{
		assert("TEMPO SETTING MUST BEGIN FROM ZERO TIME" && !tempoSettings_.cbegin()->first);
		auto tempo(--tempoSettings_.upper_bound(microSeconds_));

		auto realDelta(DeltaToMicrosec(GetEvent().deltaTime, tempo->second));
		for (double deltaTime(GetEvent().deltaTime); ++tempo != tempoSettings_.cend()
			&& microSeconds_ + realDelta > tempo->first;)
		{
			const auto newMicroSec(tempo->first);
			deltaTime -= MicrosecToDelta(newMicroSec - microSeconds_, (--tempo)->second);
			microSeconds_ = newMicroSec;
			realDelta = DeltaToMicrosec(deltaTime, (++tempo)->second);
		}
		microSeconds_ += realDelta;
	}
}

void MidiTimeCalculator::ReadEvent()
{
	using boost::format;

	if (-1 == GetEvent().eventChunk.status && 0x51 == GetEvent().eventChunk.metaType)	// -1 = 0xFF
	{
		tempoSettings_.insert(make_pair(microSeconds_, GetEvent().eventChunk.metaData));

		const auto	totalSeconds(static_cast<unsigned long>(microSeconds_) / TrackEvent::microSec),
			milliSeconds(static_cast<unsigned long>(microSeconds_) % TrackEvent::microSec * 10
				/ TrackEvent::microSec),
			minutes(totalSeconds / TrackEvent::minute),
			seconds(totalSeconds % TrackEvent::minute);

		log_ += (format{ "Time %d:%02d:%02d   Tempo = %d Beats per Minute\n" } %
			minutes % seconds % milliSeconds %
			(TrackEvent::microSec * TrackEvent::minute / GetEvent().eventChunk.metaData)).str();
	}
	else if (0x0'90 == (GetEvent().eventChunk.status & 0x0'F0)	// 0xF0 is negative ==> 0x0F0 is positive
					&& GetEvent().eventChunk.velocity)			// if velocity = 0 ==> "note-off" event
	{
		milliSeconds_.back().emplace_back(static_cast<unsigned>(microSeconds_ / 1'000));
		notes_.back().push_back(GetEvent().eventChunk.note);
	}
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

			if (currentTrack_ == 1)
			{
				assert("Global tempo map has not been initialized correctly"
					&& globalTempoSettings_.empty());
				globalTempoSettings_ = tempoSettings_;
			}
			else
			{
				assert("First track does not contain tempo information, all times will be zero"
					&& !globalTempoSettings_.empty());
				tempoSettings_ = globalTempoSettings_;
			}
		}
	}
	return result;
}

TrackEvent MidiTimeCalculator::GetEvent() const
{
	return tracks_.at(currentTrack_).at(currentEvent_);
}