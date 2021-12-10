#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "KStudent.h"
#include "KTemplateLinkedList.h"
#include <stdio.h>
class KFileIO
{
public:
	KStudent						m_Ks;
	KTemplateLinkedList<KStudent>	m_List;
public :
	void	FileCreate();
	void	FileDeleteAll();
	bool	FileLoad();
	bool	FileSave();
	void	FileDraw();
};

