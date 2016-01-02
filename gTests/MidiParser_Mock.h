# pragma once
# include "..\MidiParser\IMidiParser.h"

namespace MidiStruct
{
	struct ChunkIntro;
	struct HeaderData;
	struct TrackEvent;
}

namespace gTest
{
	class MidiParser_Mock : public IMidiParser
	{
		static int counter_;
	public:
		static void IncreaseCounter()
		{
			++counter_;
		}

		MidiParser_Mock() = default;
		virtual ~MidiParser_Mock() override final = default;

		virtual const MidiStruct::ChunkIntro ReadChunkIntro() const override final;
		virtual const MidiStruct::HeaderData ReadHeaderData() const override final;

		virtual void SkipTrackEvents(uint32_t length) const override final
		{
			ADD_FAILURE() << "Corrupted MIDI Track Header, " << length << " bytes skipped";
		}
		virtual std::vector<MidiStruct::TrackEvent> ReadTrackEvents(uint32_t) override final
		{
			return std::vector<MidiStruct::TrackEvent>();
		}

		virtual std::string GetLogAndFlush() override final
		{
			return "";
		}
		virtual std::string GetTrackNameAndFlush() override final
		{
			return "";
		}
	};
}