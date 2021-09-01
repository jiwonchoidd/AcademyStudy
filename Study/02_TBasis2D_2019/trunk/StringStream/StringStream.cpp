// StringStream.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include "pch.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>//std::setprecision
using namespace std;

class MyStringBuffer : public stringbuf
{
public:
	uint8_t &operator[](size_t index)
	{
		uint8_t *b = (uint8_t *)pbase();
		return b[index];
	}
	size_t size()
	{
		return pptr() - pbase();
	}
};

struct dHeader
{
	uint32_t    len;
	uint32_t    type;	
	uint32_t    bodyNum;
};
typedef struct {
	dHeader	ph;
	char	msg[1024];
}UPACKET, *P_UPACKET;
#define PACKET_HEADER_SIZE 12
//std::ostream& operator<<(std::ostream& out, const dHeader& h)
//{
//	return out << h.len << " " << h.type << " " << h.bodyNum;
//}
//
//std::istream& operator>>(std::istream& in, dHeader& h) 
//{
//	dHeader values;
//	in >> values.len >> values.type >> values.bodyNum;
//	h = std::move(values);	
//	return in;
//}
std::ostream& operator<<(std::ostream& out, dHeader& h)
{
	out.write(reinterpret_cast<char*>(&h), sizeof(dHeader));
	return out;
}

std::istream& operator>>(std::istream& in, dHeader& h) 
{
	dHeader values; 
	in.read(reinterpret_cast<char*>(&values), sizeof(dHeader));
	h = std::move(values);
	return in;
}
std::ostream& operator<<(std::ostream& out, UPACKET& h)
{
	out.write(reinterpret_cast<char*>(&h.ph), sizeof(dHeader));
	out.write(h.msg, h.ph.len - PACKET_HEADER_SIZE);
	return out;
}

std::istream& operator>>(std::istream& in, UPACKET& h)
{
	UPACKET uPacket;
	in.read(reinterpret_cast<char*>(&uPacket.ph), sizeof(dHeader));
	in.read(uPacket.msg, uPacket.ph.len - PACKET_HEADER_SIZE);
	h = std::move(uPacket);
	return in;
}

static std::stringstream MakePacket(int iType, stringstream& data) 
{
	stringstream SendStream;
	dHeader header;
	header.type = iType;
	header.len = PACKET_HEADER_SIZE + data.str().length();
	SendStream << header;
	SendStream << data.str();
	return std::move(SendStream);
}

int main()
{
	stringstream ssData;
	ssData << "kgca";
	int iSize = ssData.str().length();
	cout << iSize;
	
	stringstream ssDataB;
	ssDataB << "kgca" << endl;
	iSize = ssDataB.str().length();
	cout << iSize;

	vector<string> input(3, "");	
	input[0] = "1 홍길동 23 67 89";
	input[1] = "2 김개똥 100 44 34";
	input[2] = "3 말쑥이 55 77 99 ";

	for (int i = 0; i < input.size(); i++)
	{
		int num;
		int kor,eng,mat;
		string name;

		stringstream ss;	
		ss.str(input[i]);

		ss >> num >> name >> kor >> eng >> mat;
		cout << num 
			<< " " << name 
			<< " " << kor 
			<< " " << eng 
			<< " " << mat << endl;
	}

	std::string input2 = "abc,def,ghi";
	std::stringstream ss2(input2);		
	std::string token;
	while (std::getline(ss2, token, ',')) {
		std::cout << token << '\n';
	}

	double a = 34.34566535;
	std::stringstream precisionValue;
	precisionValue << std::setprecision(6);
	precisionValue << a << std::endl;
	std::cout << precisionValue.str();

	//ios_base::in	파일을 오로지 읽는 것만 가능한 모드로 개방함.
	//ios_base::out	파일을 쓰는 것만이 가능한 모드로 개방함.
	//ios_base::ate	파일을 개방할 때 파일의 끝으로 파일 포인터를 이동시킴.
	//ios_base::app	해당 파일의 맨 끝에서부터 데이터를 추가함.
	//ios_base::trunc	파일이 있으면 해당 파일의 모든 데이터를 지우고 개방함.
	//ios_base::binary	바이너리 모드로 개방함.

	////////////////////////////////////////////////////////////////////////
	// struct
	std::stringstream Buffer(std::iostream::in | std::iostream::out);
	dHeader head;
	head.len = 99;
	head.type = 3000;
	head.bodyNum = 666;
	Buffer << head;

	dHeader recv;
	Buffer >> recv;
	std::cout << recv.len << recv.type << recv.bodyNum << std::endl;
	///////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////
	std::stringstream packet(std::iostream::in | std::iostream::out);
	UPACKET uPacket;	
	uPacket.ph.len = PACKET_HEADER_SIZE + Buffer.str().length();
	uPacket.ph.type = 1000;
	uPacket.ph.bodyNum = 1;
	packet << uPacket.ph;
	packet << Buffer.str() <<endl;	
	UPACKET packRecv;
	packet >> packRecv;

	dHeader data;
	string msg(packRecv.msg, 12);
	std::stringstream dataBuffer(msg);
	dataBuffer >> data;
	std::cout << data.len << data.type << data.bodyNum << std::endl;
	////////////////////////////////////////////////////////////////////////
	std::stringstream sendString = MakePacket(1000, dataBuffer);
	UPACKET packRecv2;
	sendString >> packRecv2;
	dHeader data2;
	string msg2(packRecv2.msg, 12);
	std::stringstream dataBuffer2(msg2);
	dataBuffer2 >> data2;
	std::cout << data2.len << data2.type << data2.bodyNum << std::endl;
	//////////////////////////////////////////////////////////////////////////
	// 두개의 스트링 병합=끝으로 이동 + 입력 + 출력
	std::stringstream ss(dataBuffer.str(),ios_base::ate | std::iostream::in | ios_base::out);
	ss << dataBuffer.str();

	dHeader dataA,dataB;
	ss >> dataA;
	ss >> dataB;
	std::cout << data.len << data.type << data.bodyNum << std::endl;
	///////////////////////////////////////////////////////////////////////////////////////
	string aa = "1+2i", bb= "1+3i";
	istringstream sa(aa), sb(bb);
	ostringstream out;
	int ra, ia, rb, ib;
	char buff;
	sa >> ra >> buff >> ia >> buff;
	sb >> rb >> buff >> ib >> buff;
	out << ra * rb - ia * ib << '+' << ra * ib + ia * rb << 'i';
	string finalRet = out.str();
	/////////////////////////////////////////////////////////////////////////////////////
	int n;
	std::istringstream in;  // could also use in("1 2")
	in.rdbuf()->str("1 2"); // set the get area
	in >> n;
	std::cout << "after reading the first int from \"1 2\", the int is "
		<< n << ", str() = \"" << in.rdbuf()->str() << "\"\n"; // or in.str()

	std::ostringstream out2("1 2");
	out2 << 3;
	std::cout << "after writing the int '3' to output stream \"1 2\""
		<< ", str() = \"" << out2.str() << "\"\n";

	std::ostringstream ate("1 2", std::ios_base::ate); // C++11
	ate << 3;
	std::cout << "after writing the int '3' to append stream \"1 2\""
		<< ", str() = \"" << ate.str() << "\"\n";

	//after reading the first int from "1 2", the int is 1, str() = "1 2"
	//after writing the int '3' to output stream "1 2", str() = "3 2"
	//after writing the int '3' to append stream "1 2", str() = "1 23"
	/////////////////////////////////////////////////////////////////////////////////////
	int test[2];
	stringstream result;
	int value;

	for (int i = 0; i < 2; i++) 
	{
		result.clear();
		result.str("");
		value = i;
		result << value;
		result >> test[i];
	}
	///////////////////////////////////////////////////////////////////////////////////////
	//        파일모드상수            C언어 파일모드                          
	//ios_base::out | ios_base::trunc	"w"
		//파일을 쓰는 것만이 가능한 모드로 개방함.
		//파일이 없으면 새 파일을 만들고,
		//파일이 있으면 해당 파일의 모든 데이터를 지우고 개방함.

	//ios_base::out | ios_base::app	"a"
		//파일을 쓰는 것만이 가능한 모드로 개방함.
		//파일이 없으면 새 파일을 만들고,
		//파일이 있으면 해당 파일의 맨 끝에서부터 데이터를 추가함.

	//ios_base::in | ios_base::out	"r+"	파일을 읽고 쓰는 것이 둘 다 가능한 모드로 개방함.
	//ios_base::in | ios_base::out | ios_base::trunc	"w+"
		//파일을 읽고 쓰는 것이 둘 다 가능한 모드로 개방함.
		//파일이 없으면 새 파일을 만들고,
		//파일이 있으면 해당 파일의 모든 데이터를 지우고 개방함.
}
