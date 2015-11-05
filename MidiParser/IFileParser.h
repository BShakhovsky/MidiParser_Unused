# pragma once

class IFileParser abstract : private boost::noncopyable
{
public:
	virtual int GetBytesRemained() const = 0;
	virtual void SetBytesRemained(int value) const = 0;

	virtual int PeekByte() = 0;
	virtual char ReadByte() = 0;
	virtual void ReadData(char* data, std::streamsize count) = 0;
	virtual void SkipData(std::streamoff offset) = 0;

	virtual unsigned ReadInverse(unsigned nBytes, bool toCheck = true) = 0;
	virtual unsigned ReadVarLenFormat() = 0;

	virtual void CloseFile() = 0;

	virtual ~IFileParser() = 0 {}
protected:
	IFileParser() = default;
};