# pragma once

namespace MidiStruct
{
	struct ChunkIntro;
	struct HeaderData;
	struct HeaderChunk;
	struct TrackChunk;
}

class MidiChunksReader : private boost::noncopyable
{
	MidiChunksReader() = delete;
public:
	explicit MidiChunksReader(const char* fileName);					// for use in production
	explicit MidiChunksReader(const wchar_t* fileName);
	explicit MidiChunksReader(std::unique_ptr<class IMidiParser> mock);	// for unit tests
	~MidiChunksReader();

	static uint32_t SMPTE_TicksPerSec(uint32_t division, bool toPrint = false);

	const MidiStruct::HeaderChunk ReadHeaderChunk();					// may throw
	const MidiStruct::TrackChunk ReadTrackChunk();
					// skips alien chunks (non "MTrk"), pImpl_::ReadEvent() may throw

	std::string GetLogAndFlush();
	const std::string& GetTrackName() const
	{
		return trackName_;
	}
private:
	const std::unique_ptr<IMidiParser> pImpl_;
	std::string log_, trackName_;
};