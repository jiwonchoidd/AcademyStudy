#pragma once
#include "TNpc.h"


typedef std::map<tGUID, TCharacter, GUIDComparer> TCharacterList;

class TActiveCharacter : public TServerObj
{
private:
	TCharacterList  m_List;
public:
	TCharacterList& Get();
	void  Delete(tGUID tGuid);
	void  ReleaseAll();
	void  ChangeSuperUser(tGUID tGuid);
	void  SyncCharacters(TCharacter& tChar);
	void  SpawnCharacters(TCharacter& tChar);
	bool  ReqZoneEntry(TCharacter& tChar);
	void  AckZoneEntry(TCharacter& tChar);
public:
	TActiveCharacter();
	virtual ~TActiveCharacter();
	friend ostream& operator<<(ostream &stream, TActiveCharacter& info);
	friend istream &operator>>(istream &stream, TActiveCharacter& info);
};

