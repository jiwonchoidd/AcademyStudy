#pragma once
#include <windows.h>
#include <iostream>
#include <sstream>
#include <iomanip>//std::setprecision
using namespace std;


struct FVector
{
	float x, y, z;
	/** A zero vector (0,0,0) */
	static const FVector ZeroVector;
	/** One vector (1,1,1) */
	static const FVector OneVector;
	/** World up vector (0,0,1) */
	static const FVector UpVector;
	/** Unreal forward vector (1,0,0) */
	static const FVector ForwardVector;
	/** Unreal right vector (0,1,0) */
	static const FVector RightVector;

	FVector() { x = y = z = 0.0f; };
	FVector(float InX, float InY, float InZ) { x = InX;  y = InY;  z = InZ; };
};
struct FRotator
{
	float x, y, z;
	static const FRotator ZeroRotator;
	FRotator() { x = y = z = 0.0f; };
	FRotator(float InX, float InY, float InZ) { x = InX;  y = InY;  z = InZ; };
};



static ostream& operator<<(ostream &stream, const FVector& vector)
{
	stream.write(reinterpret_cast<const char*>(&vector), sizeof(FVector));
	return stream;
}
static ostream& operator<<(ostream &stream, const FRotator& rot)
{
	stream.write(reinterpret_cast<const char*>(&rot), sizeof(FRotator));
	return stream;
}
static istream& operator>>(istream &stream, FVector& vector)
{
	stream.read(reinterpret_cast<char*>(&vector), sizeof(FVector));
	return stream;
}
static istream& operator>>(istream &stream, FRotator& rot)
{
	stream.read(reinterpret_cast<char*>(&rot), sizeof(FRotator));
	return stream;
}