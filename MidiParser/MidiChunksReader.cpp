# include "stdafx.h"
# include "MidiChunksReader.h"
# include "MidiParser.h"
# include "MidiStruct.h"

using namespace std;
using namespace MidiStruct;

#pragma warning(push)
#pragma warning(disable:4711)	// automatic inline expansion
MidiChunksReader::MidiChunksReader(const char* fileName) :			// for use in production
	pImpl_(make_unique<MidiParser>(fileName)),
	log_(), trackName_()
{}
MidiChunksReader::MidiChunksReader(const wchar_t* fileName) :		// for use in production
	pImpl_(make_unique<MidiParser>(fileName)),
	log_(), trackName_()
{}
MidiChunksReader::MidiChunksReader(unique_ptr<IMidiParser> mock) :	// for unit tests
	pImpl_(mock.release()),
	log_(), trackName_()
{}
MidiChunksReader::~MidiChunksReader() {}
#pragma warning(pop)

string MidiChunksReader::GetLogAndFlush()
{
	const auto result(log_);
	log_.clear();
	return std::move(result);
}

void CheckHeaderIntro(const ChunkIntro intro)
{
	if (intro.type != ChunkIntro::HEADER)	throw MidiError("CORRUPTED MIDI FILE HEADER");
	if (intro.length != (sizeof HeaderData::format + sizeof HeaderData::tracks
		+ sizeof HeaderData::division))		throw MidiError("CORRUPTED MIDI FILE HEADER LENGTH");
}

uint32_t MidiChunksReader::SMPTE_TicksPerSec(const uint32_t division, const bool toPrint)
{
	const auto
		ticks(division & 0x0'00'FF),
		frames((division & 0x0'FF'00 >> Bytes::byteSize) * (-1)),
		result(ticks * frames);

	if (toPrint && (frames != 24 || frames != 25 || frames != 29 || frames != 30))
		WARNING("Wrong frames per second SMPTE time");
//		"Time-code-based time: ticks per frame = " + ticks + ", frames per second SMPTE time = "
//			+ frames + "\n\tTherefore, ticks per second = " + ticks + " * " + frames + " = " + result;
	return result;
}

const HeaderChunk MidiChunksReader::ReadHeaderChunk()
{
	HeaderChunk result;
	result.intro = pImpl_->ReadChunkIntro();
	CheckHeaderIntro(result.intro);
	result.data = pImpl_->ReadHeaderData();
	log_ = pImpl_->GetLogAndFlush();

	if (result.data.format == 0)
	{
		if (1 != result.data.tracks)
			throw MidiError("CORRUPTED MIDI FILE TRACK NUMBERS");
	}
	else if (result.data.format != 1 && result.data.format != 2)
		throw MidiError("CORRUPTED MIDI FILE FORMAT");

	if (!result.data.division)
		throw MidiError("TIME DIVISION IS ZERO, NOT REALLY SURE WHAT IT MEANS");
	else if (result.data.division & 0x80'00)
		MidiChunksReader::SMPTE_TicksPerSec(result.data.division, true);
//	else
//	{
//		log_ += "Metrical time: Pulses per Quarter Note = " + result.data.division;
//		log_ += " (pulses are MIDI imaginary time units)";
//	}

	return result;
}


const TrackChunk MidiChunksReader::ReadTrackChunk()
{
	TrackChunk result{ pImpl_->ReadChunkIntro() };
	if (ChunkIntro::TRACK != result.intro.type)
		pImpl_->SkipTrackEvents(result.intro.length);
	else
		result.trackEvent =	pImpl_->ReadTrackEvents(result.intro.length);

	log_ = pImpl_->GetLogAndFlush();
	trackName_ = pImpl_->GetTrackNameAndFlush();
	return result;
}