# pragma once

namespace MidiStruct
{
	struct ChunkIntro;
	struct HeaderData;
	struct TrackEvent;
}

class IMidiParser abstract : private boost::noncopyable
{
public:
	virtual const MidiStruct::ChunkIntro ReadChunkIntro() const = 0;
	virtual const MidiStruct::HeaderData ReadHeaderData() const = 0;

	virtual void SkipTrackEvents(uint32_t length) const = 0;
	virtual std::vector<MidiStruct::TrackEvent> ReadTrackEvents(uint32_t length) = 0;

	virtual std::string GetLogAndFlush() = 0;
	virtual std::string GetTrackNameAndFlush() = 0;

	virtual ~IMidiParser() = 0 {}
protected:
	IMidiParser() = default;
};