# pragma once

namespace MidiStruct
{
	struct TrackEvent;
}

class MidiTimeCalculator : private boost::noncopyable
{
	uint16_t tempoDivision_, UNUSED_[3];	// 6 padding bytes
	double microSeconds_;
	std::map<double, uint32_t> tempoSettings_;

	std::vector<std::vector<MidiStruct::TrackEvent>> tracks_;
	typedef std::vector<std::vector<unsigned>> Times_;
	typedef std::vector<std::vector<int16_t>> Notes_;
	Times_ milliSeconds_;
	Notes_ notes_;

	size_t currentTrack_, currentEvent_;

	std::string log_;
	std::vector<std::string> trackNames_;
public:
	MidiTimeCalculator();
	~MidiTimeCalculator();

	void LoadMidiData(const char* fileName);
	void LoadMidiData(const wchar_t* fileName);
	void CalcDeltaTimes();

	Times_ GetTimes() const
	{
		return milliSeconds_;
	}
	Notes_ GetNotes() const
	{
		return notes_;
	}
	const std::string& GetLog() const
	{
		return log_;
	}
	const std::vector<std::string>& GetTrackNames() const
	{
		return trackNames_;
	}
private:
	bool EndOfTracks();

	uint32_t GetTempo() const;
	MidiStruct::TrackEvent GetEvent() const;
};