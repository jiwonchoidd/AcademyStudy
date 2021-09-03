#ifndef TBASIS_TPROTOCOL
#define TBASIS_TPROTOCOL
#include "TMath.h"

enum class CONNECT_TYPE
{
	C_NOCONNECT,		//UMETA(DisplayName = "NoConnect"),
	C_CONNECT,		//UMETA(DisplayName = "Connect"),
	C_LOGIN,			//UMETA(DisplayName = "Login"),
	C_LOGOUT,		//UMETA(DisplayName = "Logout"),
	C_LOBBYLOGIN,	//UMETA(DisplayName = "Lobby-Login"),
	C_LOBBYLOGOUT,	//UMETA(DisplayName = "Lobby-Logout"),
	C_GAMEPLAYING,	//UMETA(DisplayName = "GamePlaying"),
	C_OK_VERIFICATION, //UMETA(DisplayName = "OK_VERIFICATION"),
	C_NO_VERIFICATION, //UMETA(DisplayName = "FAILED_VERIFICATION"),
};


template<typename T>
std::string tostr(const T& value)
{
	std::ostringstream tmp_str;
	tmp_str << value;
	return tmp_str.str();
}

template<typename T, typename ... Args >
std::string tostr(const T& value, const Args& ... args)
{
	return to_str(value) + to_str(args...);
}

struct tGUID : public GUID
{
	friend std::ostream& operator<<(std::ostream& out, const tGUID& h)
	{
		int iSize = sizeof(tGUID);
		out.write(reinterpret_cast<const char*>(&h), iSize);
		return out;
	}
	
	friend std::istream& operator>>(std::istream& in, tGUID& h)
	{
		in.read(reinterpret_cast<char*>(&h), sizeof(tGUID));
		return in;
	}

	/*friend ostream& operator<<(ostream &stream, const tGUID& tGuid)
	{
		stream << tGuid.Data1 << "";
		stream << tGuid.Data2 << "";
		stream << tGuid.Data3 << "";
		stream << tGuid.Data4[0] << "";
		stream << tGuid.Data4[1] << "";
		stream << tGuid.Data4[2] << "";
		stream << tGuid.Data4[3] << "";
		stream << tGuid.Data4[4] << "";
		stream << tGuid.Data4[5] << "";
		stream << tGuid.Data4[6] << "";
		stream << tGuid.Data4[7] << "";
		return stream;
	}

	friend istream& operator>>(istream& stream, tGUID& tGuid)
	{
		stream >> DWORD(tGuid.Data1);
		stream >> USHORT(tGuid.Data2);
		stream >> USHORT(tGuid.Data3);
		stream >> UCHAR(tGuid.Data4[0]);
		stream >> UCHAR(tGuid.Data4[1]);
		stream >> UCHAR(tGuid.Data4[2]);
		stream >> UCHAR(tGuid.Data4[3]);
		stream >> UCHAR(tGuid.Data4[4]);
		stream >> UCHAR(tGuid.Data4[5]);
		stream >> UCHAR(tGuid.Data4[6]);
		stream >> UCHAR(tGuid.Data4[7]);
		return stream;
	}*/
};
struct GUIDComparer
{
	bool operator()(const tGUID & Left, const tGUID & Right) const
	{
		return memcmp(&Left, &Right, sizeof(Right)) < 0;
	}
};

#pragma  pack(push, 1 )
typedef struct {
	uint16_t        iotype;	// ��Ŷ ����� ����
	uint16_t    	len;	// msg ����Ʈ ũ��
	uint16_t    	type;	// � ��Ŷ�̴�.	
	uint32_t        time;   // ��Ŷ ���� �ð� 
}PACKET_HEADER;

typedef struct {
	PACKET_HEADER	ph;
	char			msg[1024];
}UPACKET, *P_UPACKET;
#define PACKET_HEADER_SIZE 10

typedef struct {
	WORD	user_idx;
	WORD	posX;
	WORD	posY;
	WORD	direction; //0 ~7 8����
}TPACKET_USER_POSITION;
typedef struct {
	int m_iID;
	int m_iType;
	char m_name[20];
}TPACKET_CREATE_CHARACTER_INFO;

typedef struct {
	tGUID  tGuid;
	FVector vPosition;
	FVector vVelocity;
	FRotator vRotation;
	float	fHealth;
	bool	IsAlive;
	bool	IsAttacking;
}TPACKET_CHARACTER_INFO;

typedef struct
{
	int     iType;
	FVector vPosition;
}TPACKET_TARGETPOINT_INFO;

#pragma pack(pop)

enum TPACKET_TYPE
{
	PACKET_VERIFICATION_REQ = 1,
	PACKET_VERIFICATION_ACK,

	PACKET_CHAT_NAME_REQ = 1000, // ��ȭ�� �Է¿�û	
	PACKET_CHAT_NAME_ACK, // ��ȭ�� �Է� ����
	PACKET_CHAT_MSG, // ���� ä�� �޼���.
	PACKET_LOGIN_REQ, // �α��ο�û
	PACKET_LOGOUT_REQ, // �α׾ƿ���û
	PACKET_LOGIN_ACK, // �α�������
	PACKET_LOGOUT_ACK, // �α׾ƿ�����
	PACKET_LOGOUT_PLAYER, // �α׾ƿ� �÷��̾�

	PACKET_CREATE_ACCOUNT_REQ = 2000, // ����������û
	PACKET_DELETE_ACCOUNT_REQ, // ����������û
	PACKET_CREATE_ACCOUNT_ACK, // ������������
	PACKET_DELETE_ACCOUNT_ACK, // ������������

	PACKET_USER_POSITION = 3000, // �ɸ��� ��ġ �̵� ����
	PACKET_STOP_POSITION,// �ɸ��� ����
	PACKET_LOBBY_USER_INFO, // ���� ��������Ʈ ����
	

	PACKET_CREATE_CHARACTER_REQ = 4000, // �ɸ��� ���� ��û
	PACKET_DELETE_CHARACTER_REQ, // �ɸ��� ���� ��û
	PACKET_CREATE_CHARACTER_ACK, // �ɸ��� ���� ����
	PACKET_DELETE_CHARACTER_ACK, // �ɸ��� ���� ����

	PACKET_HIT_CHARACTER = 5000, // �ɸ��� �ǰ� ����
	PACKET_HIT_MONSTER, // ���� �ǰ� ����
	PACKET_ATTACK_CHARACTER, // �ɸ��� ���� ����
	PACKET_ATTACK_MONSTER, // ���� ���� ����
	PACKET_DEAD_CHARACTER, // �ɸ��� ����
	PACKET_DEAD_MONSTER, // ���� ����
	PACKET_DAMAGE_CHARACTER, // �ɸ��� ������ ����
	PACKET_DAMAGE_MONSTER, // ���� ������ ����
	PACKET_SPAWN_CHARACTER, // �ɸ��� ����
	PACKET_SPAWN_MONSTER, // ���� ����
	PACKET_SYNC_CHARACTER, // �ɸ��� ����ȭ
	PACKET_SYNC_MONSTER, // ���� ����ȭ
	PACKET_DESTROY_MONSTER,  // NPC �Ҹ�
	PACKET_NPC_TARGETMOVE,  // NPC Ÿ���̵�	


	PACKET_ZONE_ENTRY_REQ = 6000,//������ ���� ��û
	PACKET_ZONE_ENTRY_ACK, // ���� �� ���� ����
	PACKET_ZONE_NEW_PLAYER, // ���� �� ���� �ɸ��� ���� ����
	PACKET_ZONE_PLAYERS_INFO, // ���� ���� ������ �������� �÷��� �ɸ��� ���� ���� 
	PACKET_ZONE_USER_INFO,// ������ ��������Ʈ ����(�÷��̾�+��Ÿ(������ ��))
	PACKET_ZONE_TARGET_INFO, // �� Ÿ�� ��ġ  
};
template<typename T>
std::istream & tBlockRead(std::istream& stream, T& value)
{
	return stream.read(reinterpret_cast<char*>(&value), sizeof(T));
}
template<typename T>
std::ostream & tBlockWrite(std::ostream& stream, T& value)
{
	return stream.write(reinterpret_cast<char*>(&value), sizeof(T));
}

static std::ostream& operator<<(std::ostream &stream, const TPACKET_CHARACTER_INFO& tChar)
{
	stream.write(reinterpret_cast<const char*>(&tChar), sizeof(TPACKET_CHARACTER_INFO));
	return stream;
}

static std::istream& operator>>(std::istream& stream, TPACKET_CHARACTER_INFO& retChar)
{
	TPACKET_CHARACTER_INFO tChar;
	stream.read(reinterpret_cast<char*>(&tChar), sizeof(TPACKET_CHARACTER_INFO));
	retChar = std::move(tChar);
	return stream;
}

static std::ostream& operator<<(std::ostream& out, const PACKET_HEADER& h)
{
	out.write(reinterpret_cast<const char*>(&h), sizeof(PACKET_HEADER));
	return out;
}

static std::istream& operator>>(std::istream& in, PACKET_HEADER& h)
{
	PACKET_HEADER values;
	in.read(reinterpret_cast<char*>(&values), sizeof(PACKET_HEADER));
	h = std::move(values);
	return in;
}
static std::ostream& operator<<(std::ostream& out, const UPACKET& h)
{
	out.write(reinterpret_cast<const char*>(&h.ph), sizeof(PACKET_HEADER));
	out.write(h.msg, h.ph.len - PACKET_HEADER_SIZE);
	return out;
}
static std::istream& operator>>(std::istream& in, UPACKET& h)
{
	in.read(reinterpret_cast<char*>(&h.ph), sizeof(PACKET_HEADER));
	in.read(h.msg, h.ph.len - PACKET_HEADER_SIZE);
	return in;
}
//static std::istream& operator>>(std::istream& in, UPACKET& h)
//{
//	UPACKET uPacket;
//	in.read(reinterpret_cast<char*>(&uPacket.ph), sizeof(PACKET_HEADER));
//	in.read(uPacket.msg, uPacket.ph.len - PACKET_HEADER_SIZE);
//	h = std::move(uPacket);
//	return in;
//}

static std::stringstream MakePacket(int iType, stringstream& data)
{
	stringstream SendStream;
	PACKET_HEADER header;
	header.iotype = 777;
	header.type = iType;
	header.len = PACKET_HEADER_SIZE + data.str().length();
	SendStream << header;
	SendStream << data.str();
	return std::move(SendStream);
}

#endif