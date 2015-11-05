# pragma once
# include "IMidiParser.h"

namespace MidiStruct
{
	struct ChunkType;
}

class MidiParser : public IMidiParser
{
	std::shared_ptr<class IFileParser> inputFile_;

	MidiParser() = delete;
public:
	explicit MidiParser(const char* fileName);
	virtual ~MidiParser() override final;

	virtual const MidiStruct::ChunkIntro ReadChunkIntro() const override final;
	virtual const MidiStruct::HeaderData ReadHeaderData() const override final;

	virtual void SkipTrackEvents(uint32_t length) const override final;
	virtual std::vector<MidiStruct::TrackEvent> ReadTrackEvents(uint32_t length) const override final;
		// may throw std::length_error
private:
	const MidiStruct::ChunkType ReadChunkType() const;
};