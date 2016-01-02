# pragma once

namespace MidiStruct
{
	struct HeaderData;
	struct TrackEvent;
}

class MidiTracksCollector : private boost::noncopyable
{
	typedef std::shared_ptr<MidiStruct::HeaderData> HeaderData_;
	typedef std::vector<std::vector<MidiStruct::TrackEvent>> Tracks_;

	MidiTracksCollector() = delete;
public:
	explicit MidiTracksCollector(const char* fileName);
	explicit MidiTracksCollector(const wchar_t* fileName);
	~MidiTracksCollector();

	HeaderData_ GetHeaderData() const
	{
		return headerData_;
	}
	Tracks_ GetTracks() const
	{
		return tracks_;
	}
	const std::string& GetLog() const
	{
		return log_;
	}
	const std::vector<std::string>& GetTrackNames() const
	{
		return trackNames_;
	}

	void ReadMidiFile();
private:
	void ReadTracks();

	const std::unique_ptr<class MidiChunksReader> midiFile_;
	HeaderData_ headerData_;
	Tracks_ tracks_;
	std::string log_;
	std::vector<std::string> trackNames_;
};