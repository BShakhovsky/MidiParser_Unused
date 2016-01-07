# include "stdafx.h"
# include "MidiParser_Facade.h"

int main()
{
	using std::clock; 

	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

	auto timeStart(clock());
	const MidiParser_Facade midi("../../Test.mid");
	auto runDuration(clock() - timeStart);

	timeStart = clock();
	for (auto iter(midi.GetNotes().cbegin()); iter != midi.GetNotes().cend(); ++iter)
	{
		printf("TRACK #%d of %u\n", static_cast<int>(iter - midi.GetNotes().cbegin() + 1),
			static_cast<unsigned>(midi.GetNotes().size()));
		for (auto jiter(iter->cbegin()); jiter != iter->cend(); ++jiter)
		{
			printf("Time %u Note %d\t", midi.GetMilliSeconds()
				.at(static_cast<size_t>(iter - midi.GetNotes().cbegin()))
				.at(static_cast<size_t>(jiter - iter->cbegin())),
				*jiter);
		}
		printf("\nEND OF TRACK #%d of %u\n\n", static_cast<int>(iter - midi.GetNotes().cbegin() + 1),
			static_cast<unsigned>(midi.GetNotes().size()));
	}
	puts("===================\nEND OF ALL TRACKS\n===================");
	const auto printDuration(clock() - timeStart);
	printf("\nRun time = %f seconds\tPrint time = %f seconds\nTotal time = %f seconds\n",
		static_cast<float>(runDuration) / CLOCKS_PER_SEC, static_cast<float>(printDuration) / CLOCKS_PER_SEC,
		static_cast<float>(runDuration + printDuration) / CLOCKS_PER_SEC);
}