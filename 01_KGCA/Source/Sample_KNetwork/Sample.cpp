#define IP_DD		"192.168.0.56"
#define IP_KGCA2	"192.168.0.12"
#define IP_1		"192.168.31.236"
#define _CRT_SECURE_NO_WARNINGS
#include "Sample.h"
LRESULT  Sample::ExternMsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return m_Net.Select_MsgProc(hWnd, msg, wParam, lParam);
}
bool Sample::Init()
{
    m_Camera.CreateViewMatrix(KVector3(0, 0, -5), KVector3(0, 0, 0));
    m_Camera.CreateProjMatrix(1.0f, 1000.0f, XM_PI * 0.45f, (float)g_rtClient.right / (float)g_rtClient.bottom);

	m_Net.InitNetwork();
	//Ÿ�� ��Ʈ ������
	if(m_Net.Connect(g_hWnd, SOCK_STREAM, 10000, IP_DD))
	{
		m_bConnect = true;
	}
    return true;
}
static char chatItems[2048] = {0,};
static char buffer[MAX_PATH];
std::string	isConnect = "DisConnected";
ImVec4 color = ImVec4(1.0f,1.0f,1.0f,1.0f);
// ������ �Լ�
bool Sample::Frame()
{
	
	// ��Ŷ Ǯ ������� ä�� ���� üũ
	int iChatCnt = m_Net.m_User.m_lPacketPool.size();
	(m_Net.m_bConnect)? isConnect = "Online" : isConnect = "Offline";
	(m_Net.m_bConnect)? color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);

	#pragma region IMGUI INTERFACE
	if (ImGui::Begin("Chatting Box"))
	{
		
		ImGui::TextColored(color,isConnect.c_str());
		//������ �ȵǾ� ������ ���� �� �õ�
		if (!m_Net.m_bConnect)
		{
			if (ImGui::Button(u8"���� ��õ�"))
			{
				if (m_Net.Connect(g_hWnd, SOCK_STREAM, 10000, IP_DD))
				{
					m_bConnect = true;
				}
			}
		}
		else
		{
			ImGui::BeginChild(u8"ä��â", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing() - 15));
			ImGui::Text(chatItems);
			ImGui::EndChild();
			ImGui::Dummy(ImVec2(0.0f, 5));
			ImGui::InputText("", buffer, sizeof(buffer));
			ImGui::SameLine();
			if (ImGui::Button("Send"))
			{
				char clear[MAX_PATH] = { 0, };
				KPacket kPacket(PACKET_CHAT_MSG);
				kPacket << 123 << "Test" << (short)12 << buffer;

				//���� ���� 0���� ������ ���۵��� �ʾ���
				if (m_Net.SendMsg(m_Net.m_Sock, kPacket.m_uPacket) < 0)
				{
					ZeroMemory(&chatItems, sizeof(char) * 2048);
					strcat(chatItems, "Error\n");
					m_Net.m_bConnect = false;
				}

				strcpy(buffer, clear);
			}
		}
	}
	ImGui::End();
#pragma endregion

	if (iChatCnt > 0 && m_iChatCount != iChatCnt)
	{
		m_iChatCount = iChatCnt;
		char clear[2048] = { 0, };
		strcpy(chatItems, clear);

		std::list<KPacket>::iterator iter;
		//��ȭ���� ���ѷ��� ������ ������
		if (m_Net.m_User.m_lPacketPool.size() > 10)
		{
			m_Net.m_User.m_lPacketPool.pop_front();
		}
		for (iter = m_Net.m_User.m_lPacketPool.begin();
			iter != m_Net.m_User.m_lPacketPool.end();
			iter++)
		{
			KChatting recvdata;
			ZeroMemory(&recvdata, sizeof(recvdata));
			(*iter) >> recvdata.index >> recvdata.name
				>> recvdata.damage >> recvdata.message;
			strcat(chatItems, recvdata.name);
			strcat(chatItems, " : ");
			strcat(chatItems, recvdata.message);
			strcat(chatItems, "\n");
		
			(*iter).Reset();
		}
	}
    return true;
}
bool Sample::Render()
{
    DrawTestBox(g_fSecTimer * m_Speed,
        0.0f, 0.0f, 0.0f, nullptr, &m_Camera.m_matView, &m_Camera.m_matProj);

    return true;
}
bool Sample::Release()
{
    m_Camera.Release();
	m_Net.CloseNetwork();
    return true;
}

Sample::Sample()
{
}

Sample::~Sample()
{
}

WinMain_OPT(ä��â Ŭ���̾�Ʈ, 800, 600);