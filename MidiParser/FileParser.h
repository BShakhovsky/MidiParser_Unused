# pragma once
# include "IFileParser.h"
# include "FileCounter.h"
# include "MidiError.h"

class FileParser : public IFileParser
{
	const int32_t UNUSED = NULL;	// four padding bytes
	std::ifstream inputFile_;
	std::shared_ptr<class FileCounter> bytesRemained_;

	FileParser() = delete;
public:
	template<typename T>
	explicit FileParser(T fileName) : IFileParser(),
		inputFile_(fileName, std::ifstream::binary),
		bytesRemained_(std::make_shared<FileCounter>())
	{
		if (inputFile_.fail()) throw MidiError("Cannot open input file");
	}

	virtual ~FileParser() override final = default;

	virtual void CloseFile() override final;

	virtual int GetBytesRemained() const override final;
	virtual void SetBytesRemained(int value) const override final;

	virtual int PeekByte() override final;
	virtual char ReadByte() override final;
	virtual void ReadData(char* data, std::streamsize count) override final;
	virtual void SkipData(std::streamoff offset) override final;
			
	virtual unsigned ReadInverse(unsigned nBytes, bool toCheck) override final;
	virtual unsigned ReadVarLenFormat() override final;	// may throw
};

uint32_t ReadWord(std::shared_ptr<IFileParser> fileParser);		// Word = 4 bytes!!!
uint16_t ReadDWord(std::shared_ptr<IFileParser> fileParser);	// DWord = 2 bytes!!!