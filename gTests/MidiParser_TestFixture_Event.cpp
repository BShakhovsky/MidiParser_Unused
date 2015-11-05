# include "stdafx.h"
# include "MidiParser_TestFixture_Event.h"
# include "..\MidiParser\Event.h"
# include "..\MidiParser\MidiStruct.h"
# include "MidiParser_FileParser_Mock.h"

using std::make_shared;
using gTest::TestFixture_Event;

TestFixture_Event::TestFixture_Event(const char* fileName, const int bytesRemained) : Test(),
	file_(make_shared<FileParser_Mock>(fileName)),
	bytesRemained_(bytesRemained),
	result_(nullptr)
{}

TestFixture_Event::~TestFixture_Event() {}

void TestFixture_Event::SetUp()
{
	using testing::FLAGS_gtest_break_on_failure;

	FLAGS_gtest_break_on_failure = true;

# ifdef _DEBUG
	file_->SetBytesRemained(bytesRemained_);
# endif
	ASSERT_NO_FATAL_FAILURE(Event::GetInstance(file_)) << R"%(	1st line ("# include "stdafx.h"\n") = 0x00	)%";
	ASSERT_NO_FATAL_FAILURE(Event::GetInstance(file_)) << R"%(	2nd line ("/*******************\n") = 0x00	)%";
}

void TestFixture_Event::TearDown()
{
	ASSERT_DEBUG_DEATH(Event::GetInstance(file_), "LOGICAL ERROR IN COUNTING BYTES REMAINED TO READ FROM MIDI FILE")
														<< file_->GetBytesRemained() << " bytes left";
}