# include "stdafx.h"
# include "FileParser.h"
# include "MidiStruct.h"

using std::shared_ptr;

void FileParser::CloseFile()
{
	inputFile_.close();
	if (inputFile_.fail()) throw MidiError("CANNOT CLOSE INPUT FILE");
}

int FileParser::GetBytesRemained() const
{
	return bytesRemained_->Get();
}
void FileParser::SetBytesRemained(const int value) const
{
	bytesRemained_->Set(value);
}

# define CHECK_FLAGS {	if (inputFile_.eof()) throw MidiError("END OF INPUT FILE IS REACHED");	\
					else if (inputFile_.fail()) throw MidiError(__FUNCTION__);					}

int FileParser::PeekByte()
{
	const auto result(inputFile_.peek());
	CHECK_FLAGS;
	return result;
}
char FileParser::ReadByte()
{
	char result('\0');
	inputFile_.get(result);
	CHECK_FLAGS;
	bytesRemained_->Reduce(1);
	return result;
}
void FileParser::ReadData(char* data, const std::streamsize count)
{
	inputFile_.read(data, count);
	CHECK_FLAGS;
	bytesRemained_->Reduce(static_cast<int>(count), false);
}
void FileParser::SkipData(const std::streamoff offset)
{
	inputFile_.clear();	// remove EOF flag if set
	inputFile_.seekg(offset, std::ifstream::cur);
	CHECK_FLAGS;
	bytesRemained_->Reduce(static_cast<int>(offset));
}

unsigned FileParser::ReadInverse(unsigned nBytes, const bool toCheck)
// Inverse from little-endian to big-endian format
{
	if (nBytes > sizeof(int32_t))
	{
		assert(!"NUMBER OF BYTES TO READ > SIZEOF INT");
		nBytes = sizeof(int32_t);
	}
	unsigned result(NULL);
	for (unsigned i(NULL); i < nBytes; ++i)
	{
		result <<= MidiStruct::Bytes::byteSize;
		result |= inputFile_.get();
		CHECK_FLAGS;
	}
	bytesRemained_->Reduce(static_cast<signed>(nBytes), toCheck);
	return result;
}
unsigned FileParser::ReadVarLenFormat()
{
	using MidiStruct::Bytes;

	unsigned result(NULL);
	char anotherByte('\0');
	auto totalBytes(NULL);
	do
	{
		anotherByte = ReadByte();
		if (++totalBytes > Bytes::varLengthSize)
			throw MidiError("UNEXPECTED VARIABLE LENGTH > FOUR BYTES");
		result <<= Bytes::byteSize - 1;
		result |= static_cast<unsigned char>(anotherByte & 0x7F);
	} while (anotherByte < 0);	// ends when the most significant bit is unset

	return result;
}

uint32_t ReadWord(shared_ptr<IFileParser> fileParser)
{
	return fileParser->ReadInverse(sizeof uint32_t, false);
}
uint16_t ReadDWord(shared_ptr<IFileParser> fileParser)
{
	return static_cast<uint16_t>(fileParser->ReadInverse(sizeof uint16_t, false));
}