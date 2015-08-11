/**
 * ZIP Input-File
 */
TStrStrH TZipIn::FExtToCmdH;
const int TZipIn::MxBfL = 32 * 1024;

void TZipIn::CreateZipProcess(const TStr& Cmd, const TStr& ZipFNm) {
	TStr CmdLine = TStr::Fmt("%s %s", Cmd.CStr(), ZipFNm.CStr());
	if (Silent) CmdLine += " 2>/dev/null";
	ZipStdoutRd = popen(CmdLine.CStr(), "r");
	EAssertR(ZipStdoutRd != NULL, TStr::Fmt("Can not execute '%s'", CmdLine.CStr()).CStr());
}

void TZipIn::FillBf() {
	size_t BytesRead = fread(Bf, 1, MxBfL, ZipStdoutRd);
	BfL = (int) BytesRead;
	CurFPos += BytesRead;
	BfC = 0;
}

TZipIn::TZipIn(const TStr& FNm, const bool& Silent):
	TSBase(FNm.CStr()), TSIn(FNm), ZipStdoutRd(NULL), ZipStdoutWr(NULL),
	FLen(0), CurFPos(0), Bf(NULL), BfC(0), BfL(0), Silent(Silent) {

	CreateZipProcess(GetCmd(FNm), FNm);
	Bf = new char[MxBfL];
	BfC = BfL = -1;
	FillBf();
}

TZipIn::TZipIn(const TStr& FNm, bool& OpenedP, const bool& Silent):
	TSBase(FNm.CStr()), TSIn(FNm), ZipStdoutRd(NULL), ZipStdoutWr(NULL),
	FLen(0), CurFPos(0), Bf(NULL), BfC(0), BfL(0), Silent(Silent) {

	OpenedP = TFile::Exists(FNm);
	if (OpenedP) {
		CreateZipProcess(GetCmd(FNm.GetFExt()), FNm);
		Bf = new char[MxBfL];
		BfC = BfL = -1;
		FillBf();
	}
}

TZipIn::~TZipIn() {
	if (ZipStdoutRd != NULL) EAssertR(pclose(ZipStdoutRd) != -1, "Closing of the process failed");
	if (Bf != NULL) delete[] Bf;
}

int TZipIn::GetBf(const void* LBf, const TSize& LBfL) {
	int LBfS = 0;
	if (TSize(BfC + LBfL) > TSize(BfL)) {
		for (TSize LBfC = 0; LBfC < LBfL; LBfC++) {
			if (BfC == BfL) FillBf();
			LBfS += ((char*) LBf)[LBfC] = Bf[BfC++];
		}
	} else {
		for (TSize LBfC = 0; LBfC < LBfL; LBfC++)
			LBfS += (((char*) LBf)[LBfC] = Bf[BfC++]);
	}
	return LBfS;
}

void TZipIn::AddZipExtCmd(const TStr& ZipFNmExt, const TStr& ZipCmd) {
	if (FExtToCmdH.Empty()) FillFExtToCmdH();
	FExtToCmdH.AddDat(ZipFNmExt, ZipCmd);
}

bool TZipIn::IsZipExt(const TStr& FNmExt) {
	if (FExtToCmdH.Empty()) FillFExtToCmdH();
	return FExtToCmdH.IsKey(FNmExt);
}

void TZipIn::FillFExtToCmdH() {
	// 7za decompress: "e -y -bd -so";
	const char* ZipCmd = "7za e -y -bd -so";
	if (FExtToCmdH.Empty()) {
		FExtToCmdH.AddDat(".gz", ZipCmd);
		FExtToCmdH.AddDat(".7z", ZipCmd);
		FExtToCmdH.AddDat(".rar", ZipCmd);
		FExtToCmdH.AddDat(".zip", ZipCmd);
		FExtToCmdH.AddDat(".cab", ZipCmd);
		FExtToCmdH.AddDat(".arj", ZipCmd);
		FExtToCmdH.AddDat(".bzip2", ZipCmd);
		FExtToCmdH.AddDat(".bz2", ZipCmd);
	}
}

TStr TZipIn::GetCmd(const TStr& ZipFNm) {
	if (FExtToCmdH.Empty()) FillFExtToCmdH();
	const TStr Ext = ZipFNm.GetFExt().GetLc();
	EAssertR(FExtToCmdH.IsKey(Ext), TStr::Fmt("Unsupported file extension '%s'", Ext.CStr()));
	return FExtToCmdH.GetDat(Ext);
}

uint64 TZipIn::GetFLen(const TStr& ZipFNm) {
	const TStr CmdLine = TStr::Fmt("7za l %s", ZipFNm.CStr());
	FILE* ZipStdoutRd = popen(CmdLine.CStr(), "r");
	EAssertR(ZipStdoutRd != NULL, TStr::Fmt("Can not execute '%s'", CmdLine.CStr()).CStr());
	// Read output from the child process
	const int BfSz = 32 * 1024;
	char* Bf = new char[BfSz];
	int BfC = 0, BfL = 0;
	memset(Bf, 0, BfSz);
	size_t BytesRead = fread(Bf, 1, MxBfL, ZipStdoutRd);
	EAssert(BytesRead != 0);
	EAssert(pclose(ZipStdoutRd) != -1);
	BfL = (int) BytesRead;
	IAssert((BfC != 0) || (BfL != 0));
	BfC = Bf[BfL] = 0;
	// find file lenght
	TStr Str(Bf);
	delete[] Bf;
	TStrV StrV;
	Str.SplitOnWs(StrV);
	int n = StrV.Len();
	if (n - 5 <= 0) {
		WrNotify(TStr::Fmt("Corrupt file %s: MESSAGE:\n", ZipFNm.CStr()).CStr(), Str.CStr());
		SaveToErrLog(TStr::Fmt("Corrupt file %s. Message:\n:%s\n", ZipFNm.CStr(), Str.CStr()).CStr());
		return 0;
	}
	return StrV[n - 5].GetInt64();
}

/////////////////////////////////////////////////
// Output-File
TStrStrH TZipOut::FExtToCmdH;
const TSize TZipOut::MxBfL = 4 * 1024;

void TZipOut::FlushBf() {
#ifdef GLib_WIN
	DWORD BytesOut;
	EAssertR(WriteFile(ZipStdinWr, Bf, DWORD(BfL), &BytesOut, NULL)!=0, "Error writting to the file '"+GetSNm()+"'.");
#else
	size_t BytesOut = fwrite(Bf, 1, BfL, ZipStdinWr);
#endif
	EAssert(BytesOut == BfL);
	BfL = 0;
}

void TZipOut::CreateZipProcess(const TStr& Cmd, const TStr& ZipFNm) {
	TStr CmdLine = TStr::Fmt("%s %s", Cmd.CStr(), ZipFNm.CStr());
	if (Silent) CmdLine += " >/dev/null";
#ifdef GLib_WIN
	PROCESS_INFORMATION piProcInfo;
	STARTUPINFO siStartInfo;
	ZeroMemory( &piProcInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory( &siStartInfo, sizeof(STARTUPINFO));
	siStartInfo.cb = sizeof(STARTUPINFO);
	siStartInfo.hStdInput = ZipStdinRd;
	siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
	// Create the child process.
	const BOOL FuncRetn = CreateProcess(NULL,
										(LPSTR) CmdLine.CStr(),// command line
										NULL,// process security attributes
										NULL,// primary thread security attributes
										TRUE,// handles are inherited
										0,// creation flags
										NULL,// use parent's environment
										NULL,// use parent's current directory
										&siStartInfo,// STARTUPINFO pointer
										&piProcInfo);// receives PROCESS_INFORMATION
	EAssertR(FuncRetn!=0, TStr::Fmt("Can not execute '%s'", CmdLine.CStr()).CStr());
	CloseHandle(piProcInfo.hProcess);
	CloseHandle(piProcInfo.hThread);
#else
	ZipStdinWr = popen(CmdLine.CStr(), "w");
	EAssertR(ZipStdinWr != NULL, TStr::Fmt("Can not execute '%s'", CmdLine.CStr()).CStr());
#endif
}

TZipOut::TZipOut(const TStr& FNm, const bool& Silent):
	TSBase(FNm.CStr()), TSOut(FNm), ZipStdinRd(NULL), ZipStdinWr(NULL),
	Bf(NULL), BfL(0), Silent(Silent) {

	EAssertR(!FNm.Empty(), "Empty file-name.");
#ifdef GLib_WIN
	// create pipes
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;
	// Create a pipe for the child process's STDOUT.
	EAssertR(CreatePipe(&ZipStdinRd, &ZipStdinWr, &saAttr, 0), "Stdout pipe creation failed");
	// Ensure the read handle to the pipe for STDOUT is not inherited.
	SetHandleInformation(ZipStdinWr, HANDLE_FLAG_INHERIT, 0);
#else
	// no implementation necessary
#endif
	CreateZipProcess(GetCmd(FNm), FNm);
	Bf = new char[MxBfL];
	BfL = 0;
}

TZipOut::~TZipOut() {
	Close();
	if (Bf != NULL) delete[] Bf;
}

void TZipOut::Close() {
	if (BfL != 0) FlushBf();
#ifdef GLib_WIN
	if (ZipStdinWr != NULL) {
		EAssertR(CloseHandle(ZipStdinWr), "Closing write-end of pipe failed");
		ZipStdinWr = NULL;
	}
	if (ZipStdinRd != NULL) {
		EAssertR(CloseHandle(ZipStdinRd), "Closing read-end of pipe failed");
		ZipStdinRd = NULL;
	}
#else
	if (ZipStdinWr != NULL) {
		EAssertR(pclose(ZipStdinWr) != -1, "Closing of the process failed");
		ZipStdinWr = NULL;
	}
#endif
}

int TZipOut::PutCh(const char& Ch) {
	if (BfL == MxBfL) FlushBf();
	return Bf[BfL++] = Ch;
}

int TZipOut::PutBf(const void* LBf, const TSize& LBfL) {
	int LBfS = 0;
	if (BfL + LBfL > MxBfL) {
		for (TSize LBfC = 0; LBfC < LBfL; LBfC++) LBfS += PutCh(((char*) LBf)[LBfC]);
	} else {
		for (TSize LBfC = 0; LBfC < LBfL; LBfC++) LBfS += (Bf[BfL++] = ((char*) LBf)[LBfC]);
	}
	return LBfS;
}

void TZipOut::Flush() {
	FlushBf();
#ifdef GLib_WIN
	EAssertR(FlushFileBuffers(ZipStdinWr)!=0, "Can not flush file '"+GetSNm()+"'.");
#else
	EAssertR(fflush(ZipStdinWr) == 0, "Can not flush file '" + GetSNm() + "'.");
#endif
}

void TZipOut::AddZipExtCmd(const TStr& ZipFNmExt, const TStr& ZipCmd) {
	if (FExtToCmdH.Empty())	FillFExtToCmdH();
	FExtToCmdH.AddDat(ZipFNmExt, ZipCmd);
}

bool TZipOut::IsZipExt(const TStr& FNmExt) {
	if (FExtToCmdH.Empty())	FillFExtToCmdH();
	return FExtToCmdH.IsKey(FNmExt);
}

void TZipOut::FillFExtToCmdH() {
	// 7za compress: "a -y -bd -si{CompressedFNm}"
#ifdef GLib_WIN
	const char* ZipCmd = "7z.exe a -y -bd -si";
#else
	const char* ZipCmd = "7za a -y -bd -si";
#endif
	if (FExtToCmdH.Empty()) {
		FExtToCmdH.AddDat(".gz", ZipCmd);
		FExtToCmdH.AddDat(".7z", ZipCmd);
		FExtToCmdH.AddDat(".rar", ZipCmd);
		FExtToCmdH.AddDat(".zip", ZipCmd);
		FExtToCmdH.AddDat(".cab", ZipCmd);
		FExtToCmdH.AddDat(".arj", ZipCmd);
		FExtToCmdH.AddDat(".bzip2", ZipCmd);
		FExtToCmdH.AddDat(".bz2", ZipCmd);
	}
}

TStr TZipOut::GetCmd(const TStr& ZipFNm) {
	if (FExtToCmdH.Empty()) FillFExtToCmdH();
	const TStr Ext = ZipFNm.GetFExt().GetLc();
	EAssertR(FExtToCmdH.IsKey(Ext),	TStr::Fmt("Unsupported file extension '%s'", Ext.CStr()));
	return FExtToCmdH.GetDat(Ext) + ZipFNm.GetFMid();
}
