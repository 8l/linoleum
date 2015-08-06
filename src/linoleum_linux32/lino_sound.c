#include "lino_sound.h"

/**
 * handles all PCM commands.
 * @return false when errors, true otherwise
 */
bool krnlPCMdataCommand(PCMdataCommand command)
{
	if (command != IDLE) {
		PRINT1("File PCMdata: %u\n", command);
		return false;
	}
	return true;
}
