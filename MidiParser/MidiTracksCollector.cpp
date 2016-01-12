# include "stdafx.h"
# include "MidiTracksCollector.h"
# include "MidiChunksReader.h"
# include "MidiStruct.h"

using namespace std;

MidiTracksCollector::MidiTracksCollector(const char* fileName) :
	midiFile_(make_unique<MidiChunksReader>(fileName)),
	headerData_(nullptr),
	tracks_(),
	log_(),
	trackNames_()
{}
MidiTracksCollector::MidiTracksCollector(const wchar_t* fileName) :
	midiFile_(make_unique<MidiChunksReader>(fileName)),
	headerData_(nullptr),
	tracks_(),
	log_(),
	trackNames_()
{}
MidiTracksCollector::~MidiTracksCollector() {}

void MidiTracksCollector::ReadMidiFile()
{
	using MidiStruct::HeaderData;

	if (!tracks_.empty()) { BORIS_ASSERT(__FUNCTION__ " MUST NOT BE CALLED TWICE"); }
	else
	headerData_ = make_shared<HeaderData>(midiFile_->ReadHeaderChunk().data);
	ReadTracks();
	log_ += midiFile_->GetLogAndFlush();
}

void MidiTracksCollector::ReadTracks()
{
	using boost::format;

	tracks_.reserve(headerData_->tracks);
	trackNames_.reserve(headerData_->tracks);
	for (uint16_t i(0); i < headerData_->tracks; ++i)
	{
		const auto result(midiFile_->ReadTrackChunk().trackEvent);
		if (!result.empty()) tracks_.push_back(result);	// may throw
		trackNames_.push_back(midiFile_->GetTrackName());

		const auto log(midiFile_->GetLogAndFlush());
		if (!log.empty()) log_ += (format{ "Track %1%: " } % (i + 1)).str() + log;
	}
}