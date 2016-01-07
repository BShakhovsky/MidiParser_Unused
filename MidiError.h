#pragma once

class MidiError : public std::exception
{
public:
	explicit MidiError(const char* _Message)
		: std::exception(_Message),
		msg(_Message)
	{}
	virtual ~MidiError() throw() override final = default;
	virtual const char* what() const override final
	{
		return msg.c_str() ? msg.c_str() : "Unknown exception";
	}
private:
	std::string msg;

	MidiError() = delete;
};