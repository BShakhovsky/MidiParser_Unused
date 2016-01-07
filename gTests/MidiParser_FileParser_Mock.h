# pragma once
# include "MidiParser\IFileParser.h"

class FileCounter;

namespace gTest
{
	class FileParser_Mock : public IFileParser
	{
		const int32_t UNUSED = NULL;	// four padding bytes
		std::ifstream inputFile_;
		std::shared_ptr<FileCounter> bytesRemained_;

		FileParser_Mock() = delete;
	public:
		explicit FileParser_Mock(const char* fileName);
		virtual ~FileParser_Mock() override final = default;

		virtual void CloseFile() override final
		{
			BORIS_ASSERT(__FUNCTION__ "HAS NOT BEEN OVERRIDEN");
		}
		virtual int GetBytesRemained() const override final;
		virtual void SetBytesRemained(int value) const override final;

		virtual int PeekByte() override final;
		virtual char ReadByte() override final;
		virtual void ReadData(char* data, std::streamsize count) override final;
		virtual void SkipData(std::streamoff offset) override final;

		virtual unsigned ReadInverse(unsigned nBytes, bool toCheck) override final;
		virtual unsigned ReadVarLenFormat() override final;
	private:
		std::unique_ptr<std::string> ReadLine();
		int ReadNumber();
	};
}