#pragma once
#include <windows.h>
#include <iostream>
#include <sstream>
#include <iomanip>//std::setprecision
using namespace std;

/** Returns highest of 3 values */
template< class T >
static  T Max3(const T A, const T B, const T C)
{
	return Max(Max(A, B), C);
}

/** Returns lowest of 3 values */
template< class T >
static T Min3(const T A, const T B, const T C)
{
	return Min(Min(A, B), C);
}

/** Multiples value by itself */
template< class T >
static T Square(const T A)
{
	return A * A;
}

/** Clamps X to be between Min and Max, inclusive */
template< class T >
static T Clamp(const T X, const T Min, const T Max)
{
	return X < Min ? Min : X < Max ? X : Max;
}

struct FVector
{
	float X, Y, Z;
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
	float DistanctTo(FVector vDesk);
	FVector operator^(const FVector& V) const;
	FVector CrossProduct(const FVector& A, const FVector& B);
	float operator|(const FVector& V) const;
	float DotProduct(const FVector& A, const FVector& B);
	FVector operator+(const FVector& V) const;
	FVector operator-(const FVector& V) const;
	FVector operator-(float Bias) const;
	FVector operator+(float Bias) const;
	FVector operator*(float Scale) const;
	FVector operator/(float Scale) const;
	FVector operator*(const FVector& V) const;
	FVector operator/(const FVector& V) const;
	bool operator==(const FVector& V) const;
	bool operator!=(const FVector& V) const;
	bool Equals(const FVector& V, float Tolerance) const;
	bool AllComponentsEqual(float Tolerance) const;
	FVector operator-() const;
	FVector operator+=(const FVector& V);
	FVector operator-=(const FVector& V);
	FVector operator*=(float Scale);
	FVector operator/=(float V);
	FVector operator*=(const FVector& V);
	FVector operator/=(const FVector& V);
	float& operator[](int Index);
	float operator[](int Index)const;
	void Set(float InX, float InY, float InZ);
	float Size() const;
	float SizeSquared() const;
	bool IsZero() const;
	bool Normalize(float Tolerance);
	float Dist(const FVector &V1, const FVector &V2);
	float DistanceTo(FVector vDesk);
	float DistSquared(const FVector &V1, const FVector &V2);
	FVector() { X = Y = Z = 0.0f; };
	FVector(float InX, float InY, float InZ) { X = InX;  Y = InY;  Z = InZ; };
};
struct FRotator
{
	float X, Y, Z;
	static const FRotator ZeroRotator;
	FRotator() { X = Y = Z = 0.0f; };
	FRotator(float InX, float InY, float InZ) { X = InX;  Y = InY;  Z = InZ; };
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