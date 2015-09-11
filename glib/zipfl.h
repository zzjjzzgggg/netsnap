#ifndef zipfl_h
#define zipfl_h

/////////////////////////////////////////////////
// Includes
//#include "base.h"

// #define TFileIn(FNm)
//   (TZipIn::IsZipFNm((FNm)) ? TZipIn(FNm) : TFIn(FNm));
// #define TFileOut(FNm)
//   TZipOut::IsZipFNm(FNm) ? TZipOut(FNm) : TFOut(FNm);

/////////////////////////////////////////////////
// Compressed input and output streams.
// 7za.exe or 7z.exe must be in the path (http://www.7-zip.org)
// 7za.exe is a stand-alone program, which supports
//   -- extraction:  .gz, .7z, .rar, .zip, .cab, .arj. bzip2
//   -- compression: .7z
// 7z.exe uses DLLs in folders Codecs and Formats
//   -- extraction:  .gz, .7z, .rar, .zip, .cab, .arj. bzip2
//   -- compression: .gz, .7z, .rar, .zip, .cab, .arj. bzip2
// Add this to Windows registry to locate Codecs and Formats:
//   [HKEY_LOCAL_MACHINE\SOFTWARE\7-Zip]
//   "Path"="C:\\Programs\\Util"

/////////////////////////////////////////////////
// ZIP Input-File (requires 7za.exe or 7z.exe)
class TZipIn: public TSIn {
private:
	static TStrStrH FExtToCmdH;
	static const int MxBfL;
	FILE* ZipStdoutRd, *ZipStdoutWr;
	uint64 FLen, CurFPos;
	char* Bf;
	int BfC, BfL;
	bool Silent;
private:
	void FillBf();
	void CreateZipProcess(const TStr& Cmd, const TStr& ZipFNm);
private:
	TZipIn();
	TZipIn(const TZipIn&);
	TZipIn& operator=(const TZipIn&);
public:
	TZipIn(const TStr& FNm, const bool& Silent=true);
	TZipIn(const TStr& FNm, bool& OpenedP, const bool& Silent);
	static PSIn New(const TStr& FNm, const bool& Silent=true) { return PSIn(new TZipIn(FNm, Silent)); }
	static PSIn New(const TStr& FNm, bool& OpenedP, const bool& Silent) { return PSIn(new TZipIn(FNm, OpenedP, Silent)); }
	~TZipIn();

	bool Eof() {return BfL<MxBfL && BfC == BfL;}
	int Len() const {return int(FLen - CurFPos + BfL - BfC);}
	char GetCh() { if (BfC == BfL) FillBf(); return Bf[BfC++]; }
	char PeekCh() { if (BfC == BfL) FillBf(); return Bf[BfC];}
	int GetBf(const void* LBf, const TSize& LBfL);

	uint64 GetFLen() const {return FLen; }
	uint64 GetCurFPos() const {return CurFPos; }

	static void AddZipExtCmd(const TStr& ZipFNmExt, const TStr& ZipCmd);
	static bool IsZipFNm(const TStr& FNm) { return IsZipExt(FNm.GetFExt());}
	static bool IsZipExt(const TStr& FNmExt);
	static void FillFExtToCmdH();
	static TStr GetCmd(const TStr& ZipFNm);
	static uint64 GetFLen(const TStr& ZipFNm);
};

/////////////////////////////////////////////////
// ZIP Output-File (requires 7z.exe and 7za.exe)
class TZipOut: public TSOut {
private:
	static const TSize MxBfL;
	static TStrStrH FExtToCmdH;
#ifdef GLib_WIN
	HANDLE ZipStdinRd, ZipStdinWr;
#else
	FILE *ZipStdinRd, *ZipStdinWr;
#endif
	char* Bf;
	TSize BfL;
	bool Silent;
private:
	void FlushBf();
	void CreateZipProcess(const TStr& Cmd, const TStr& ZipFNm);
private:
	TZipOut();
	TZipOut(const TZipOut&);
	TZipOut& operator=(const TZipOut&);
public:
	TZipOut(const TStr& _FNm, const bool& Silent=false);
	static PSOut New(const TStr& FNm, const bool& Silent=false) { return PSOut(new TZipOut(FNm, Silent)); }
	~TZipOut();

	int PutCh(const char& Ch);
	int PutBf(const void* LBf, const TSize& LBfL);
	void Flush();
	void Close();

	static void AddZipExtCmd(const TStr& ZipFNmExt, const TStr& ZipCmd);
	static bool IsZipFNm(const TStr& FNm) { return IsZipExt(FNm.GetFExt()); }
	static bool IsZipExt(const TStr& FNmExt);
	static void FillFExtToCmdH();
	static TStr GetCmd(const TStr& ZipFNm);
};

#endif
