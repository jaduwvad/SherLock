#pragma once
class fileTreat
{
public:
	fileTreat(void);
	~fileTreat(void);

	CString conFileName;			//받은 파일의 이름 저장
	CString conFilePath;			//받은 파일의 경로 저장

	void DeleteData(fileTreat filelist[], CString filepath);
	void AlignIndex(fileTreat filelist[]);
	void DeleteAllData(fileTreat existFileList[]);
	void InsertData(fileTreat filelist[], CString filename, CString filepath);
	void OnFileload();
	void addFileMemory(CString filePath);
	void DeleteFileMemory();
	bool DataCheck(fileTreat filelist[], CString filename, CString filepath);
	void initlist(fileTreat existfile[]);
	void listAditing(fileTreat filelist[]);
	BOOL LockingFile(CString filePath, HANDLE* hFile);
	BOOL unLockingFile(CString filePath, HANDLE* hFile);
	void passwardPass(fileTreat filelist[], HANDLE h_file[]);
	void passwardUnPassed(fileTreat filelist[], HANDLE h_file[]);
};

