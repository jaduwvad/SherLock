#pragma once
#include "StdAfx.h"
#include "fileTreat.h"


fileTreat::fileTreat(void)
{
}


fileTreat::~fileTreat(void)
{
}

void fileTreat::DeleteAllData(fileTreat existFileList[])
{
	CFile filelist;

	for(int i=0; i<256; i++){
		if(existFileList[i].conFileName=="")
			break;

		else{
			existFileList[i].conFileName="";
			existFileList[i].conFilePath="";
		}
	}

	filelist.Open("D:\\listmemory\\lockfilelist.txt",CFile::modeCreate);
	filelist.Write("", 0);
	
	filelist.Close();
}

void fileTreat::AlignIndex(fileTreat filelist[]){
	int i,j;

	for(i=0; i<256; i++){
		if(filelist[i].conFileName==""){
			for(j=i; j<256; j++){
				if(filelist[j].conFileName!=""){
					filelist[i].conFileName=filelist[j].conFileName;
					filelist[i].conFilePath=filelist[j].conFilePath;
					filelist[j].conFileName="";
					filelist[j].conFilePath="";
					AlignIndex(filelist);
				}
			}
		}
	}
}	//�迭�� �߰��߰� ������ ���� �� �����͵��� ������ ���Ľ��� �ִ� �Լ�

void fileTreat::DeleteData(fileTreat filelist[], CString filepath){
	for(int i=0; i<256; i++){
		if(filelist[i].conFilePath==filepath){
			filelist[i].conFileName="";
			filelist[i].conFilePath="";
			return;
		}
	}
	return;
}	//�޾ƿ� ������ ��θ� ���� �迭�� ����ִ� ������ ����

void fileTreat::InsertData(fileTreat filelist[], CString filename, CString filepath)
{


	for(int i=0; i<512; i++){
		if(filelist[i].conFilePath==""){
			filelist[i].conFileName=filename;
			filelist[i].conFilePath=filepath;
			break;
		}
	}
}	//�޾ƿ� ������ �̸��� ������ �迭�� ����

bool fileTreat::DataCheck(fileTreat filelist[], CString filename, CString filepath)
{
	int i;

	for(i=0; i<256; i++){
		if(filelist[i].conFileName==filename)
			return FALSE;
	
		else
			continue;
	}
	return TRUE;
}	//�޾ƿ� ������ ������ �迭�� ����Ǿ� �ִ��� Ȯ��

void fileTreat::OnFileload() 
{
	CFileFind findingfile;
	CFile file;
	CString file_adr=_T("D:\\listmemory\\lockfilelist.txt");
	BOOL filecheck=findingfile.FindFile(_T("D:\\listmemory\\lockfilelist.txt"));

	if(filecheck==TRUE){
		return;
	}

	else if(filecheck==FALSE){
		CreateDirectory(_T("D:\\listmemory"),NULL);
		file.Open(file_adr,CFile::modeCreate);
		file.Close();
	}
}

void fileTreat::addFileMemory(CString filePath)
{
	CFile file;
	CString filelist;
	filelist=filePath+'\n';

	file.Open(_T("D:\\listmemory\\lockfilelist.txt"), CFile::modeReadWrite);
	file.SeekToEnd();
	file.Write(filelist,filelist.GetLength());
	file.Close();
}


void fileTreat::initlist(fileTreat existfile[])
{
	CStdioFile filelist;
	CString listcheck;
	CString filedata[256], filenamebuf;
	char* charbuf;
	CFile filebuf;
	ULONGLONG lengthbuf;
	int j=0,i=0;
	int curserbuf=0;

	filelist.Open("D:\\listmemory\\lockfilelist.txt", CFile::modeRead);
	lengthbuf=filelist.GetLength();
	charbuf=new char[lengthbuf];
	filelist.Read(charbuf, lengthbuf);
	listcheck=(CString)charbuf;

	filelist.Close();
	charbuf=NULL;

	while(1){
		curserbuf=listcheck.Find('\n',curserbuf+1);

		if(curserbuf==-1)
			break;

		else{
			AfxExtractSubString(filedata[i], listcheck, i, '\n');
			i++;
		}
	}

	for(int a=0; a<i; a++){
		filebuf.Open(filedata[a], CFile::modeRead);
		filenamebuf=filebuf.GetFileName();
		filebuf.Close();
		existfile[a].conFileName=filenamebuf;
		existfile[a].conFilePath=filedata[a];
	}
}

void fileTreat::listAditing(fileTreat filelist[])
{
	CFile listfile,file2;
	CString listadit;

	for(int i=0; i<256; i++){
		if(filelist[i].conFileName!="")
			listadit=listadit+filelist[i].conFilePath+'\n';

		else if(filelist[i].conFileName==""){
			break;
		}
	}

	file2.Open("D:\\listmemory\\lockfilelist.txt", CFile::modeCreate);
	file2.Write("",0);
	file2.Close();

	listfile.Open("D:\\listmemory\\lockfilelist.txt", CFile::modeWrite);
	listfile.Write(listadit, listadit.GetLength());
	listfile.Close();
}


BOOL fileTreat::LockingFile(CString filePath, HANDLE* hFile)
{
	*hFile = CreateFile(filePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	return TRUE;
}	//����Ʈ�� �ִ� ���ϵ鿡 ����� �ɰ� �ȴ�.

BOOL fileTreat::unLockingFile(CString filePath, HANDLE* hFile)
{
	return CloseHandle(*hFile);
}	//����Ʈ�� �ִ� ���ϵ鿡 �ɸ� ����� �����Ѵ�

void fileTreat::passwardPass(fileTreat filelist[], HANDLE h_file[])
{
	for(int i=0; i<512; i++){
		if(filelist[i].conFileName=="")
			break;
		else
			unLockingFile(filelist[i].conFilePath, &h_file[i]);
	}
}

void fileTreat::passwardUnPassed(fileTreat filelist[], HANDLE h_file[])
{
	for(int i=0; i<512; i++){
		if(filelist[i].conFileName=="")
			break;

		else{
			CloseHandle(h_file[i]);
			LockingFile(filelist[i].conFilePath, &h_file[i]);
		}
	}
}
