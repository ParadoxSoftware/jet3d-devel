#include <windows.h>

void jeErrorLog_Initialize(const char* logName, const char* logDir);
void jeErrorLog_Shutdown();

BOOL WINAPI DllMain(_In_ HINSTANCE hInstDLL, _In_ DWORD fdwReason, _In_ LPVOID lpReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		jeErrorLog_Initialize("jet3d", ".");
		break;

	case DLL_PROCESS_DETACH:
		jeErrorLog_Shutdown();
		break;
	}

	return TRUE;
}