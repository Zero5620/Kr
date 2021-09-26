#include "zBase.h"
#include "zBaseCRT.h"

int main(int argc, char *argv[]) {
	InitThreadContextCrt(MegaBytes(32));

	LogInfo("Platform: %s\n", GetPlatformName());

	return 0;
}
