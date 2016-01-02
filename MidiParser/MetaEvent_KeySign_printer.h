# pragma once

class MetaEvent_KeySign_printer
{
	typedef std::shared_ptr<class IFileParser> FileParser_;
	FileParser_ inputFile_;
	int sf;
	std::string log_;
public:
	MetaEvent_KeySign_printer();
	~MetaEvent_KeySign_printer();

	void KeySignInit(FileParser_);
	void PrintKeySignature();

	const std::string& GetLog() const
	{
		return log_;
	}
private:
	void PrintKeySF();
	void PrintKeyMI();
};