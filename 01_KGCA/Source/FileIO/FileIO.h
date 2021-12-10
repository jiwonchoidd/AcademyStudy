#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
class FileIO
{
public :
	void	FileCreate();
	void	FileDeleteAll();
	bool	FileLoad();
	void	FileSave();
	void	FileDraw();
};

