# include "stdafx.h"
# include "MetaEvent_KeySign_printer.h"
# include "IFileParser.h"
# include "MidiError.h"

MetaEvent_KeySign_printer::MetaEvent_KeySign_printer() :
	inputFile_(nullptr),
	sf(NULL),
	log_("Key signature: ")
{}

MetaEvent_KeySign_printer::~MetaEvent_KeySign_printer() {}

void MetaEvent_KeySign_printer::KeySignInit(FileParser_ newFile)
{
	inputFile_ = newFile;
}



void MetaEvent_KeySign_printer::PrintKeySignature()
{
	if (!inputFile_) throw MidiError("INPUT FILE HAS NOT BEEN SET YET");
	sf = inputFile_->ReadByte();
	PrintKeySF();
	PrintKeyMI();
}

void MetaEvent_KeySign_printer::PrintKeySF()
{
	using boost::format;

	switch (sf)
	{
	case-7: case-6: case-5: case-4: case-3: case-2: case-1:
			log_ += (format{ "%1% flats, " } % sf).str();	break;

	case 0:	log_ += "natural, ";							break;

	case 1: case 2: case 3: case 4: case 5: case 6: case 7:
			log_ += (format{ "%1% sharps, " } % sf).str();	break;

	default: throw MidiError("WRONG KEY SIGNATURE, SHOULD BE 0...7 FLATS OR SHARPS");
	}
}

void MetaEvent_KeySign_printer::PrintKeyMI()
{
	if (!inputFile_) assert(!"INPUT FILE HAS NOT BEEN SET YET");
	switch (inputFile_->ReadByte())	// mi
	{
	case 0: log_ += (sf ? "major\n" : "C-Major\n");	break;
	case 1: log_ += (sf ? "minor\n" : "A-Minor\n");	break;
	default: throw MidiError("WRONG KEY SIGNATURE, SHOULD BE EITHER MAJOR OR MINOR");
	}
}