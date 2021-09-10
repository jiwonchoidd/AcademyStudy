#include "TMath.h"

const FVector FVector::ZeroVector(0.0f, 0.0f, 0.0f);
const FVector FVector::OneVector(1.0f, 1.0f, 1.0f);
const FVector FVector::UpVector(0.0f, 0.0f, 1.0f);
const FVector FVector::ForwardVector(1.0f, 0.0f, 0.0f);
const FVector FVector::RightVector(0.0f, 1.0f, 0.0f);
const FRotator FRotator::ZeroRotator(0.f, 0.f, 0.f);

FVector FVector::operator^(const FVector& V) const
{
	return FVector
	(
		Y * V.Z - Z * V.Y,
		Z * V.X - X * V.Z,
		X * V.Y - Y * V.X
	);
}

FVector FVector::CrossProduct(const FVector& A, const FVector& B)
{
	return A ^ B;
}

float FVector::operator|(const FVector& V) const
{
	return X * V.X + Y * V.Y + Z * V.Z;
}

float FVector::DotProduct(const FVector& A, const FVector& B)
{
	return A | B;
}

FVector FVector::operator+(const FVector& V) const
{
	return FVector(X + V.X, Y + V.Y, Z + V.Z);
}

FVector FVector::operator-(const FVector& V) const
{
	return FVector(X - V.X, Y - V.Y, Z - V.Z);
}

FVector FVector::operator-(float Bias) const
{
	return FVector(X - Bias, Y - Bias, Z - Bias);
}

FVector FVector::operator+(float Bias) const
{
	return FVector(X + Bias, Y + Bias, Z + Bias);
}

FVector FVector::operator*(float Scale) const
{
	return FVector(X * Scale, Y * Scale, Z * Scale);
}

FVector FVector::operator/(float Scale) const
{
	const float RScale = 1.f / Scale;
	return FVector(X * RScale, Y * RScale, Z * RScale);
}

FVector FVector::operator*(const FVector& V) const
{
	return FVector(X * V.X, Y * V.Y, Z * V.Z);
}

FVector FVector::operator/(const FVector& V) const
{
	return FVector(X / V.X, Y / V.Y, Z / V.Z);
}

bool FVector::operator==(const FVector& V) const
{
	return X == V.X && Y == V.Y && Z == V.Z;
}

bool FVector::operator!=(const FVector& V) const
{
	return X != V.X || Y != V.Y || Z != V.Z;
}

bool FVector::Equals(const FVector& V, float Tolerance) const
{
	return fabs(X - V.X) <= Tolerance && fabs(Y - V.Y) <= Tolerance && fabs(Z - V.Z) <= Tolerance;
}

bool FVector::AllComponentsEqual(float Tolerance) const
{
	return fabs(X - Y) <= Tolerance && fabs(X - Z) <= Tolerance && fabs(Y - Z) <= Tolerance;
}


FVector FVector::operator-() const
{
	return FVector(-X, -Y, -Z);
}

FVector FVector::operator+=(const FVector& V)
{
	X += V.X; Y += V.Y; Z += V.Z;
	return *this;
}

FVector FVector::operator-=(const FVector& V)
{
	X -= V.X; Y -= V.Y; Z -= V.Z;
	return *this;
}

FVector FVector::operator*=(float Scale)
{
	X *= Scale; Y *= Scale; Z *= Scale;
	return *this;
}

FVector FVector::operator/=(float V)
{
	const float RV = 1.f / V;
	X *= RV; Y *= RV; Z *= RV;
	return *this;
}

FVector FVector::operator*=(const FVector& V)
{
	X *= V.X; Y *= V.Y; Z *= V.Z;
	return *this;
}
FVector FVector::operator/=(const FVector& V)
{
	X /= V.X; Y /= V.Y; Z /= V.Z;
	return *this;
}

float& FVector::operator[](int Index)
{
	if (Index == 0)
	{
		return X;
	}
	else if (Index == 1)
	{
		return Y;
	}
	else
	{
		return Z;
	}
}

float FVector::operator[](int Index)const
{
	if (Index < 0 && Index >= 3) return 0.0f;
	if (Index == 0)
	{
		return X;
	}
	else if (Index == 1)
	{
		return Y;
	}
	else
	{
		return Z;
	}
}

void FVector::Set(float InX, float InY, float InZ)
{
	X = InX;
	Y = InY;
	Z = InZ;
}

float FVector::Size() const
{
	return sqrt(X*X + Y * Y + Z * Z);
}

float FVector::SizeSquared() const
{
	return X * X + Y * Y + Z * Z;
}
bool FVector::IsZero() const
{
	return X == 0.f && Y == 0.f && Z == 0.f;
}

bool FVector::Normalize(float Tolerance)
{
	const float SquareSum = X * X + Y * Y + Z * Z;
	if (SquareSum > Tolerance)
	{
		const float Scale = 1.0f / SquareSum;
		X *= Scale; Y *= Scale; Z *= Scale;
		return true;
	}
	return false;
}
float FVector::Dist(const FVector &V1, const FVector &V2)
{
	return sqrt(FVector::DistSquared(V1, V2));
}
float FVector::DistanceTo(FVector vDesk)
{
	return sqrt(FVector::DistSquared(*this, vDesk));
}
float FVector::DistSquared(const FVector &V1, const FVector &V2)
{
	return Square(V2.X - V1.X) + Square(V2.Y - V1.Y) + Square(V2.Z - V1.Z);
}
