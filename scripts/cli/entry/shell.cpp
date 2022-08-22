#include <iwindeapi/internal.h>
#include <shlobj.h>
#include <winerror.h>

extern "C" int cli_entry_shell_SwitchDesktop(int argc, wchar_t* argv[]) {
	IWDEM_CheckOrReturn(argc == 1, -1, puts("Usage: SwitchDesktop <New-Desktop-Path>"));

	if (auto hr = SHSetKnownFolderPath(FOLDERID_Desktop, 0, nullptr, argv[0]); SUCCEEDED(hr)) {
		LPITEMIDLIST list = nullptr;
		SHGetKnownFolderIDList(FOLDERID_Desktop, 0, nullptr, &list);
		SHChangeNotify(SHCNE_UPDATEDIR, SHCNF_FLUSH | SHCNF_IDLIST, list, nullptr);
	} else {
		return 1;
	}

	return 0;
}

extern "C" int cli_entry_shell(int argc, wchar_t* argv[]) {
	IWDEM_CheckOrReturn(argc > 0, 1, puts("Usage: shell <Command> [Options]"));
	if (wcscmp(argv[0], L"SwitchDesktop") == 0) {
		return cli_entry_shell_SwitchDesktop(argc - 1, argv + 1);
	} else {
		wprintf(LR"(unknown command "%S")", argv[0]);
		return 2;
	}
}