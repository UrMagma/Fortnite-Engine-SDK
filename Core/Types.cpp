/*
 * UE4 Core Types Implementation - Embedded UE4 type implementations
 * Self-contained, vibe coder approved ✨
 */

#include "Types.h"
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cctype>

namespace UE4SDK
{
    // FString implementation
    FString FString::ToUpper() const
    {
        std::string result = m_String;
        std::transform(result.begin(), result.end(), result.begin(), ::toupper);
        return FString(result);
    }
    
    FString FString::ToLower() const
    {
        std::string result = m_String;
        std::transform(result.begin(), result.end(), result.begin(), ::tolower);
        return FString(result);
    }
    
    FString FString::Trim() const
    {
        std::string result = m_String;
        result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        result.erase(std::find_if(result.rbegin(), result.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), result.end());
        return FString(result);
    }
    
    bool FString::Contains(const FString& Substring) const
    {
        return m_String.find(Substring.m_String) != std::string::npos;
    }
    
    bool FString::StartsWith(const FString& Prefix) const
    {
        return m_String.substr(0, Prefix.m_String.length()) == Prefix.m_String;
    }
    
    bool FString::EndsWith(const FString& Suffix) const
    {
        if (Suffix.m_String.length() > m_String.length()) return false;
        return m_String.substr(m_String.length() - Suffix.m_String.length()) == Suffix.m_String;
    }
    
    int32 FString::Find(const FString& Substring) const
    {
        size_t pos = m_String.find(Substring.m_String);
        return pos != std::string::npos ? static_cast<int32>(pos) : -1;
    }
    
    int32 FString::FindLast(const FString& Substring) const
    {
        size_t pos = m_String.rfind(Substring.m_String);
        return pos != std::string::npos ? static_cast<int32>(pos) : -1;
    }
    
    FString FString::Left(int32 Count) const
    {
        if (Count <= 0) return FString();
        if (Count >= static_cast<int32>(m_String.length())) return *this;
        return FString(m_String.substr(0, Count));
    }
    
    FString FString::Right(int32 Count) const
    {
        if (Count <= 0) return FString();
        if (Count >= static_cast<int32>(m_String.length())) return *this;
        return FString(m_String.substr(m_String.length() - Count));
    }
    
    FString FString::Mid(int32 Start, int32 Count) const
    {
        if (Start < 0 || Start >= static_cast<int32>(m_String.length())) return FString();
        if (Count < 0) Count = static_cast<int32>(m_String.length()) - Start;
        return FString(m_String.substr(Start, Count));
    }
    
    std::vector<FString> FString::Split(const FString& Delimiter) const
    {
        std::vector<FString> result;
        std::string str = m_String;
        std::string delim = Delimiter.m_String;
        
        size_t start = 0;
        size_t end = str.find(delim);
        
        while (end != std::string::npos)
        {
            result.push_back(FString(str.substr(start, end - start)));
            start = end + delim.length();
            end = str.find(delim, start);
        }
        
        result.push_back(FString(str.substr(start)));
        return result;
    }
    
    FString FString::Replace(const FString& From, const FString& To) const
    {
        std::string result = m_String;
        size_t pos = 0;
        while ((pos = result.find(From.m_String, pos)) != std::string::npos)
        {
            result.replace(pos, From.m_String.length(), To.m_String);
            pos += To.m_String.length();
        }
        return FString(result);
    }
    
    int32 FString::ToInt() const
    {
        return std::stoi(m_String);
    }
    
    float FString::ToFloat() const
    {
        return std::stof(m_String);
    }
    
    bool FString::ToBool() const
    {
        std::string lower = ToLower().m_String;
        return lower == "true" || lower == "1" || lower == "yes";
    }
    
    FString FString::FromInt(int32 Value)
    {
        return FString(std::to_string(Value));
    }
    
    FString FString::FromFloat(float Value)
    {
        return FString(std::to_string(Value));
    }
    
    FString FString::FromBool(bool Value)
    {
        return FString(Value ? "true" : "false");
    }
    
    FString FString::ToString() const
    {
        return *this;
    }

    // FVector implementation
    bool FVector::operator==(const FVector& Other) const
    {
        return X == Other.X && Y == Other.Y && Z == Other.Z;
    }
    
    bool FVector::operator!=(const FVector& Other) const
    {
        return !(*this == Other);
    }
    
    FVector FVector::operator+(const FVector& Other) const
    {
        return FVector(X + Other.X, Y + Other.Y, Z + Other.Z);
    }
    
    FVector FVector::operator-(const FVector& Other) const
    {
        return FVector(X - Other.X, Y - Other.Y, Z - Other.Z);
    }
    
    FVector FVector::operator*(const FVector& Other) const
    {
        return FVector(X * Other.X, Y * Other.Y, Z * Other.Z);
    }
    
    FVector FVector::operator*(float Scale) const
    {
        return FVector(X * Scale, Y * Scale, Z * Scale);
    }
    
    FVector FVector::operator/(const FVector& Other) const
    {
        return FVector(X / Other.X, Y / Other.Y, Z / Other.Z);
    }
    
    FVector FVector::operator/(float Scale) const
    {
        return FVector(X / Scale, Y / Scale, Z / Scale);
    }
    
    FVector FVector::operator-() const
    {
        return FVector(-X, -Y, -Z);
    }
    
    FVector& FVector::operator+=(const FVector& Other)
    {
        X += Other.X; Y += Other.Y; Z += Other.Z;
        return *this;
    }
    
    FVector& FVector::operator-=(const FVector& Other)
    {
        X -= Other.X; Y -= Other.Y; Z -= Other.Z;
        return *this;
    }
    
    FVector& FVector::operator*=(const FVector& Other)
    {
        X *= Other.X; Y *= Other.Y; Z *= Other.Z;
        return *this;
    }
    
    FVector& FVector::operator*=(float Scale)
    {
        X *= Scale; Y *= Scale; Z *= Scale;
        return *this;
    }
    
    FVector& FVector::operator/=(const FVector& Other)
    {
        X /= Other.X; Y /= Other.Y; Z /= Other.Z;
        return *this;
    }
    
    FVector& FVector::operator/=(float Scale)
    {
        X /= Scale; Y /= Scale; Z /= Scale;
        return *this;
    }
    
    float FVector::operator|(const FVector& Other) const
    {
        return X * Other.X + Y * Other.Y + Z * Other.Z;
    }
    
    FVector FVector::operator^(const FVector& Other) const
    {
        return FVector(
            Y * Other.Z - Z * Other.Y,
            Z * Other.X - X * Other.Z,
            X * Other.Y - Y * Other.X
        );
    }
    
    float FVector::Size() const
    {
        return std::sqrt(X * X + Y * Y + Z * Z);
    }
    
    float FVector::SizeSquared() const
    {
        return X * X + Y * Y + Z * Z;
    }
    
    float FVector::Size2D() const
    {
        return std::sqrt(X * X + Y * Y);
    }
    
    float FVector::SizeSquared2D() const
    {
        return X * X + Y * Y;
    }
    
    FVector FVector::GetSafeNormal(float Tolerance) const
    {
        float SquareSum = SizeSquared();
        if (SquareSum < Tolerance) return FVector::ZeroVector;
        return *this * (1.0f / std::sqrt(SquareSum));
    }
    
    FVector FVector::GetUnsafeNormal() const
    {
        float SquareSum = SizeSquared();
        return *this * (1.0f / std::sqrt(SquareSum));
    }
    
    bool FVector::IsNearlyZero(float Tolerance) const
    {
        return SizeSquared() < Tolerance * Tolerance;
    }
    
    bool FVector::IsZero() const
    {
        return X == 0.0f && Y == 0.0f && Z == 0.0f;
    }
    
    bool FVector::IsUnit(float LengthSquaredTolerance) const
    {
        return std::abs(SizeSquared() - 1.0f) < LengthSquaredTolerance;
    }
    
    bool FVector::ContainsNaN() const
    {
        return std::isnan(X) || std::isnan(Y) || std::isnan(Z);
    }
    
    bool FVector::IsFinite() const
    {
        return std::isfinite(X) && std::isfinite(Y) && std::isfinite(Z);
    }
    
    FVector FVector::RotateAngleAxis(float AngleDeg, const FVector& Axis) const
    {
        float AngleRad = AngleDeg * (3.14159265359f / 180.0f);
        float Cos = std::cos(AngleRad);
        float Sin = std::sin(AngleRad);
        
        FVector NormalizedAxis = Axis.GetSafeNormal();
        FVector Projected = *this - NormalizedAxis * (*this | NormalizedAxis);
        FVector Perpendicular = NormalizedAxis ^ *this;
        
        return NormalizedAxis * (*this | NormalizedAxis) + Projected * Cos + Perpendicular * Sin;
    }
    
    FString FVector::ToString() const
    {
        std::ostringstream oss;
        oss << "X=" << X << " Y=" << Y << " Z=" << Z;
        return FString(oss.str());
    }
    
    // FVector static constants
    const FVector FVector::ZeroVector(0.0f, 0.0f, 0.0f);
    const FVector FVector::OneVector(1.0f, 1.0f, 1.0f);
    const FVector FVector::UpVector(0.0f, 0.0f, 1.0f);
    const FVector FVector::DownVector(0.0f, 0.0f, -1.0f);
    const FVector FVector::ForwardVector(1.0f, 0.0f, 0.0f);
    const FVector FVector::BackwardVector(-1.0f, 0.0f, 0.0f);
    const FVector FVector::RightVector(0.0f, 1.0f, 0.0f);
    const FVector FVector::LeftVector(0.0f, -1.0f, 0.0f);

    // FRotator implementation
    bool FRotator::operator==(const FRotator& Other) const
    {
        return Pitch == Other.Pitch && Yaw == Other.Yaw && Roll == Other.Roll;
    }
    
    bool FRotator::operator!=(const FRotator& Other) const
    {
        return !(*this == Other);
    }
    
    FRotator FRotator::operator+(const FRotator& Other) const
    {
        return FRotator(Pitch + Other.Pitch, Yaw + Other.Yaw, Roll + Other.Roll);
    }
    
    FRotator FRotator::operator-(const FRotator& Other) const
    {
        return FRotator(Pitch - Other.Pitch, Yaw - Other.Yaw, Roll - Other.Roll);
    }
    
    FRotator FRotator::operator*(const FRotator& Other) const
    {
        return FRotator(Pitch * Other.Pitch, Yaw * Other.Yaw, Roll * Other.Roll);
    }
    
    FRotator FRotator::operator*(float Scale) const
    {
        return FRotator(Pitch * Scale, Yaw * Scale, Roll * Scale);
    }
    
    FRotator FRotator::operator-() const
    {
        return FRotator(-Pitch, -Yaw, -Roll);
    }
    
    FRotator& FRotator::operator+=(const FRotator& Other)
    {
        Pitch += Other.Pitch; Yaw += Other.Yaw; Roll += Other.Roll;
        return *this;
    }
    
    FRotator& FRotator::operator-=(const FRotator& Other)
    {
        Pitch -= Other.Pitch; Yaw -= Other.Yaw; Roll -= Other.Roll;
        return *this;
    }
    
    FRotator& FRotator::operator*=(const FRotator& Other)
    {
        Pitch *= Other.Pitch; Yaw *= Other.Yaw; Roll *= Other.Roll;
        return *this;
    }
    
    FRotator& FRotator::operator*=(float Scale)
    {
        Pitch *= Scale; Yaw *= Scale; Roll *= Scale;
        return *this;
    }
    
    bool FRotator::IsNearlyZero(float Tolerance) const
    {
        return std::abs(Pitch) < Tolerance && std::abs(Yaw) < Tolerance && std::abs(Roll) < Tolerance;
    }
    
    bool FRotator::IsZero() const
    {
        return Pitch == 0.0f && Yaw == 0.0f && Roll == 0.0f;
    }
    
    bool FRotator::ContainsNaN() const
    {
        return std::isnan(Pitch) || std::isnan(Yaw) || std::isnan(Roll);
    }
    
    bool FRotator::IsFinite() const
    {
        return std::isfinite(Pitch) && std::isfinite(Yaw) && std::isfinite(Roll);
    }
    
    FRotator FRotator::GetNormalized() const
    {
        return FRotator(
            std::fmod(Pitch + 360.0f, 360.0f),
            std::fmod(Yaw + 360.0f, 360.0f),
            std::fmod(Roll + 360.0f, 360.0f)
        );
    }
    
    FRotator FRotator::GetDenormalized() const
    {
        return *this;
    }
    
    FVector FRotator::Vector() const
    {
        float PitchRad = Pitch * (3.14159265359f / 180.0f);
        float YawRad = Yaw * (3.14159265359f / 180.0f);
        float RollRad = Roll * (3.14159265359f / 180.0f);
        
        float CP = std::cos(PitchRad);
        float SP = std::sin(PitchRad);
        float CY = std::cos(YawRad);
        float SY = std::sin(YawRad);
        float CR = std::cos(RollRad);
        float SR = std::sin(RollRad);
        
        return FVector(
            CP * CY,
            CP * SY,
            SP
        );
    }
    
    FVector FRotator::GetForwardVector() const
    {
        return Vector();
    }
    
    FVector FRotator::GetRightVector() const
    {
        float YawRad = Yaw * (3.14159265359f / 180.0f);
        return FVector(-std::sin(YawRad), std::cos(YawRad), 0.0f);
    }
    
    FVector FRotator::GetUpVector() const
    {
        float PitchRad = Pitch * (3.14159265359f / 180.0f);
        float YawRad = Yaw * (3.14159265359f / 180.0f);
        return FVector(
            -std::sin(PitchRad) * std::cos(YawRad),
            -std::sin(PitchRad) * std::sin(YawRad),
            std::cos(PitchRad)
        );
    }
    
    FString FRotator::ToString() const
    {
        std::ostringstream oss;
        oss << "Pitch=" << Pitch << " Yaw=" << Yaw << " Roll=" << Roll;
        return FString(oss.str());
    }
    
    // FRotator static constants
    const FRotator FRotator::ZeroRotator(0.0f, 0.0f, 0.0f);

    // FQuat implementation
    bool FQuat::operator==(const FQuat& Other) const
    {
        return X == Other.X && Y == Other.Y && Z == Other.Z && W == Other.W;
    }
    
    bool FQuat::operator!=(const FQuat& Other) const
    {
        return !(*this == Other);
    }
    
    FQuat FQuat::operator+(const FQuat& Other) const
    {
        return FQuat(X + Other.X, Y + Other.Y, Z + Other.Z, W + Other.W);
    }
    
    FQuat FQuat::operator-(const FQuat& Other) const
    {
        return FQuat(X - Other.X, Y - Other.Y, Z - Other.Z, W - Other.W);
    }
    
    FQuat FQuat::operator*(const FQuat& Other) const
    {
        return FQuat(
            W * Other.X + X * Other.W + Y * Other.Z - Z * Other.Y,
            W * Other.Y - X * Other.Z + Y * Other.W + Z * Other.X,
            W * Other.Z + X * Other.Y - Y * Other.X + Z * Other.W,
            W * Other.W - X * Other.X - Y * Other.Y - Z * Other.Z
        );
    }
    
    FQuat FQuat::operator*(float Scale) const
    {
        return FQuat(X * Scale, Y * Scale, Z * Scale, W * Scale);
    }
    
    FQuat FQuat::operator-() const
    {
        return FQuat(-X, -Y, -Z, -W);
    }
    
    FQuat& FQuat::operator+=(const FQuat& Other)
    {
        X += Other.X; Y += Other.Y; Z += Other.Z; W += Other.W;
        return *this;
    }
    
    FQuat& FQuat::operator-=(const FQuat& Other)
    {
        X -= Other.X; Y -= Other.Y; Z -= Other.Z; W -= Other.W;
        return *this;
    }
    
    FQuat& FQuat::operator*=(const FQuat& Other)
    {
        *this = *this * Other;
        return *this;
    }
    
    FQuat& FQuat::operator*=(float Scale)
    {
        X *= Scale; Y *= Scale; Z *= Scale; W *= Scale;
        return *this;
    }
    
    float FQuat::Size() const
    {
        return std::sqrt(X * X + Y * Y + Z * Z + W * W);
    }
    
    float FQuat::SizeSquared() const
    {
        return X * X + Y * Y + Z * Z + W * W;
    }
    
    FQuat FQuat::GetNormalized() const
    {
        float Size = Size();
        if (Size == 0.0f) return FQuat::Identity;
        return *this * (1.0f / Size);
    }
    
    bool FQuat::IsNormalized() const
    {
        return std::abs(SizeSquared() - 1.0f) < 0.01f;
    }
    
    bool FQuat::IsNearlyZero(float Tolerance) const
    {
        return SizeSquared() < Tolerance * Tolerance;
    }
    
    bool FQuat::IsZero() const
    {
        return X == 0.0f && Y == 0.0f && Z == 0.0f && W == 0.0f;
    }
    
    bool FQuat::ContainsNaN() const
    {
        return std::isnan(X) || std::isnan(Y) || std::isnan(Z) || std::isnan(W);
    }
    
    bool FQuat::IsFinite() const
    {
        return std::isfinite(X) && std::isfinite(Y) && std::isfinite(Z) && std::isfinite(W);
    }
    
    FVector FQuat::RotateVector(const FVector& V) const
    {
        FQuat VQuat(V.X, V.Y, V.Z, 0.0f);
        FQuat Result = *this * VQuat * GetNormalized();
        return FVector(Result.X, Result.Y, Result.Z);
    }
    
    FVector FQuat::GetAxisX() const
    {
        return RotateVector(FVector::ForwardVector);
    }
    
    FVector FQuat::GetAxisY() const
    {
        return RotateVector(FVector::RightVector);
    }
    
    FVector FQuat::GetAxisZ() const
    {
        return RotateVector(FVector::UpVector);
    }
    
    FString FQuat::ToString() const
    {
        std::ostringstream oss;
        oss << "X=" << X << " Y=" << Y << " Z=" << Z << " W=" << W;
        return FString(oss.str());
    }
    
    // FQuat static constants
    const FQuat FQuat::Identity(0.0f, 0.0f, 0.0f, 1.0f);

    // FTransform implementation
    FTransform::FTransform(const FRotator& InRotation, const FVector& InTranslation, const FVector& InScale3D)
        : Rotation(InRotation), Translation(InTranslation), Scale3D(InScale3D)
    {
        // Convert rotator to quaternion
        float PitchRad = InRotation.Pitch * (3.14159265359f / 180.0f);
        float YawRad = InRotation.Yaw * (3.14159265359f / 180.0f);
        float RollRad = InRotation.Roll * (3.14159265359f / 180.0f);
        
        float CP = std::cos(PitchRad * 0.5f);
        float SP = std::sin(PitchRad * 0.5f);
        float CY = std::cos(YawRad * 0.5f);
        float SY = std::sin(YawRad * 0.5f);
        float CR = std::cos(RollRad * 0.5f);
        float SR = std::sin(RollRad * 0.5f);
        
        Rotation = FQuat(
            CR * CP * CY + SR * SP * SY,
            CR * CP * SY - SR * SP * CY,
            CR * SP * CY + SR * CP * SY,
            SR * CP * CY - CR * SP * SY
        );
    }
    
    bool FTransform::operator==(const FTransform& Other) const
    {
        return Rotation == Other.Rotation && Translation == Other.Translation && Scale3D == Other.Scale3D;
    }
    
    bool FTransform::operator!=(const FTransform& Other) const
    {
        return !(*this == Other);
    }
    
    FTransform FTransform::operator*(const FTransform& Other) const
    {
        FTransform Result;
        Result.Rotation = Rotation * Other.Rotation;
        Result.Translation = Translation + Rotation.RotateVector(Other.Translation * Scale3D);
        Result.Scale3D = Scale3D * Other.Scale3D;
        return Result;
    }
    
    FTransform& FTransform::operator*=(const FTransform& Other)
    {
        *this = *this * Other;
        return *this;
    }
    
    FVector FTransform::TransformPosition(const FVector& V) const
    {
        return Translation + Rotation.RotateVector(V * Scale3D);
    }
    
    FVector FTransform::TransformVector(const FVector& V) const
    {
        return Rotation.RotateVector(V * Scale3D);
    }
    
    FVector FTransform::TransformVectorNoScale(const FVector& V) const
    {
        return Rotation.RotateVector(V);
    }
    
    FVector FTransform::InverseTransformPosition(const FVector& V) const
    {
        return Rotation.GetNormalized().RotateVector(V - Translation) / Scale3D;
    }
    
    FVector FTransform::InverseTransformVector(const FVector& V) const
    {
        return Rotation.GetNormalized().RotateVector(V) / Scale3D;
    }
    
    FVector FTransform::InverseTransformVectorNoScale(const FVector& V) const
    {
        return Rotation.GetNormalized().RotateVector(V);
    }
    
    FTransform FTransform::GetRelativeTransform(const FTransform& Other) const
    {
        FTransform Result;
        Result.Rotation = Other.Rotation.GetNormalized() * Rotation;
        Result.Translation = Other.Rotation.GetNormalized().RotateVector(Translation - Other.Translation) / Other.Scale3D;
        Result.Scale3D = Scale3D / Other.Scale3D;
        return Result;
    }
    
    FTransform FTransform::GetRelativeTransformReverse(const FTransform& Other) const
    {
        return Other.GetRelativeTransform(*this);
    }
    
    FTransform FTransform::GetScaled(float InScale) const
    {
        return FTransform(Rotation, Translation, Scale3D * InScale);
    }
    
    FTransform FTransform::GetScaled(const FVector& InScale) const
    {
        return FTransform(Rotation, Translation, Scale3D * InScale);
    }
    
    FTransform FTransform::GetNormalized() const
    {
        return FTransform(Rotation.GetNormalized(), Translation, Scale3D);
    }
    
    bool FTransform::IsRotationNormalized() const
    {
        return Rotation.IsNormalized();
    }
    
    bool FTransform::IsTranslationValid() const
    {
        return Translation.IsFinite();
    }
    
    bool FTransform::IsScale3DValid() const
    {
        return Scale3D.IsFinite() && Scale3D.X > 0.0f && Scale3D.Y > 0.0f && Scale3D.Z > 0.0f;
    }
    
    bool FTransform::ContainsNaN() const
    {
        return Rotation.ContainsNaN() || Translation.ContainsNaN() || Scale3D.ContainsNaN();
    }
    
    bool FTransform::IsFinite() const
    {
        return Rotation.IsFinite() && Translation.IsFinite() && Scale3D.IsFinite();
    }
    
    FString FTransform::ToString() const
    {
        std::ostringstream oss;
        oss << "Rotation=" << Rotation.ToString() << " Translation=" << Translation.ToString() << " Scale3D=" << Scale3D.ToString();
        return FString(oss.str());
    }
    
    // FTransform static constants
    const FTransform FTransform::Identity(FQuat::Identity, FVector::ZeroVector, FVector::OneVector);

    // FLinearColor implementation
    bool FLinearColor::operator==(const FLinearColor& Other) const
    {
        return R == Other.R && G == Other.G && B == Other.B && A == Other.A;
    }
    
    bool FLinearColor::operator!=(const FLinearColor& Other) const
    {
        return !(*this == Other);
    }
    
    FLinearColor FLinearColor::operator+(const FLinearColor& Other) const
    {
        return FLinearColor(R + Other.R, G + Other.G, B + Other.B, A + Other.A);
    }
    
    FLinearColor FLinearColor::operator-(const FLinearColor& Other) const
    {
        return FLinearColor(R - Other.R, G - Other.G, B - Other.B, A - Other.A);
    }
    
    FLinearColor FLinearColor::operator*(const FLinearColor& Other) const
    {
        return FLinearColor(R * Other.R, G * Other.G, B * Other.B, A * Other.A);
    }
    
    FLinearColor FLinearColor::operator*(float Scale) const
    {
        return FLinearColor(R * Scale, G * Scale, B * Scale, A * Scale);
    }
    
    FLinearColor FLinearColor::operator/(const FLinearColor& Other) const
    {
        return FLinearColor(R / Other.R, G / Other.G, B / Other.B, A / Other.A);
    }
    
    FLinearColor FLinearColor::operator/(float Scale) const
    {
        return FLinearColor(R / Scale, G / Scale, B / Scale, A / Scale);
    }
    
    FLinearColor& FLinearColor::operator+=(const FLinearColor& Other)
    {
        R += Other.R; G += Other.G; B += Other.B; A += Other.A;
        return *this;
    }
    
    FLinearColor& FLinearColor::operator-=(const FLinearColor& Other)
    {
        R -= Other.R; G -= Other.G; B -= Other.B; A -= Other.A;
        return *this;
    }
    
    FLinearColor& FLinearColor::operator*=(const FLinearColor& Other)
    {
        R *= Other.R; G *= Other.G; B *= Other.B; A *= Other.A;
        return *this;
    }
    
    FLinearColor& FLinearColor::operator*=(float Scale)
    {
        R *= Scale; G *= Scale; B *= Scale; A *= Scale;
        return *this;
    }
    
    FLinearColor& FLinearColor::operator/=(const FLinearColor& Other)
    {
        R /= Other.R; G /= Other.G; B /= Other.B; A /= Other.A;
        return *this;
    }
    
    FLinearColor& FLinearColor::operator/=(float Scale)
    {
        R /= Scale; G /= Scale; B /= Scale; A /= Scale;
        return *this;
    }
    
    FLinearColor FLinearColor::GetClamped() const
    {
        return FLinearColor(
            std::max(0.0f, std::min(1.0f, R)),
            std::max(0.0f, std::min(1.0f, G)),
            std::max(0.0f, std::min(1.0f, B)),
            std::max(0.0f, std::min(1.0f, A))
        );
    }
    
    FLinearColor FLinearColor::GetClamped(float Min, float Max) const
    {
        return FLinearColor(
            std::max(Min, std::min(Max, R)),
            std::max(Min, std::min(Max, G)),
            std::max(Min, std::min(Max, B)),
            std::max(Min, std::min(Max, A))
        );
    }
    
    bool FLinearColor::IsAlmostBlack() const
    {
        return R < 0.01f && G < 0.01f && B < 0.01f;
    }
    
    bool FLinearColor::IsAlmostWhite() const
    {
        return R > 0.99f && G > 0.99f && B > 0.99f;
    }
    
    FString FLinearColor::ToString() const
    {
        std::ostringstream oss;
        oss << "R=" << R << " G=" << G << " B=" << B << " A=" << A;
        return FString(oss.str());
    }
    
    // FLinearColor static constants
    const FLinearColor FLinearColor::White(1.0f, 1.0f, 1.0f, 1.0f);
    const FLinearColor FLinearColor::Black(0.0f, 0.0f, 0.0f, 1.0f);
    const FLinearColor FLinearColor::Red(1.0f, 0.0f, 0.0f, 1.0f);
    const FLinearColor FLinearColor::Green(0.0f, 1.0f, 0.0f, 1.0f);
    const FLinearColor FLinearColor::Blue(0.0f, 0.0f, 1.0f, 1.0f);
    const FLinearColor FLinearColor::Yellow(1.0f, 1.0f, 0.0f, 1.0f);
    const FLinearColor FLinearColor::Cyan(0.0f, 1.0f, 1.0f, 1.0f);
    const FLinearColor FLinearColor::Magenta(1.0f, 0.0f, 1.0f, 1.0f);
    const FLinearColor FLinearColor::Transparent(0.0f, 0.0f, 0.0f, 0.0f);

    // FColor implementation
    FColor::FColor(const FLinearColor& InLinearColor)
    {
        R = static_cast<uint8>(std::max(0.0f, std::min(255.0f, InLinearColor.R * 255.0f)));
        G = static_cast<uint8>(std::max(0.0f, std::min(255.0f, InLinearColor.G * 255.0f)));
        B = static_cast<uint8>(std::max(0.0f, std::min(255.0f, InLinearColor.B * 255.0f)));
        A = static_cast<uint8>(std::max(0.0f, std::min(255.0f, InLinearColor.A * 255.0f)));
    }
    
    bool FColor::operator==(const FColor& Other) const
    {
        return R == Other.R && G == Other.G && B == Other.B && A == Other.A;
    }
    
    bool FColor::operator!=(const FColor& Other) const
    {
        return !(*this == Other);
    }
    
    FLinearColor FColor::ReinterpretAsLinear() const
    {
        return FLinearColor(R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f);
    }
    
    FString FColor::ToString() const
    {
        std::ostringstream oss;
        oss << "R=" << static_cast<int>(R) << " G=" << static_cast<int>(G) << " B=" << static_cast<int>(B) << " A=" << static_cast<int>(A);
        return FString(oss.str());
    }
    
    // FColor static constants
    const FColor FColor::White(255, 255, 255, 255);
    const FColor FColor::Black(0, 0, 0, 255);
    const FColor FColor::Red(255, 0, 0, 255);
    const FColor FColor::Green(0, 255, 0, 255);
    const FColor FColor::Blue(0, 0, 255, 255);
    const FColor FColor::Yellow(255, 255, 0, 255);
    const FColor FColor::Cyan(0, 255, 255, 255);
    const FColor FColor::Magenta(255, 0, 255, 255);
    const FColor FColor::Transparent(0, 0, 0, 0);

    // TArray implementation
    template<typename T>
    void TArray<T>::RemoveAt(IndexType Index)
    {
        if (Index >= 0 && Index < static_cast<IndexType>(m_Data.size()))
        {
            m_Data.erase(m_Data.begin() + Index);
        }
    }
    
    template<typename T>
    void TArray<T>::RemoveAtSwap(IndexType Index)
    {
        if (Index >= 0 && Index < static_cast<IndexType>(m_Data.size()))
        {
            std::swap(m_Data[Index], m_Data.back());
            m_Data.pop_back();
        }
    }
    
    template<typename T>
    bool TArray<T>::Remove(const T& Item)
    {
        auto it = std::find(m_Data.begin(), m_Data.end(), Item);
        if (it != m_Data.end())
        {
            m_Data.erase(it);
            return true;
        }
        return false;
    }
    
    template<typename T>
    bool TArray<T>::RemoveSwap(const T& Item)
    {
        auto it = std::find(m_Data.begin(), m_Data.end(), Item);
        if (it != m_Data.end())
        {
            std::swap(*it, m_Data.back());
            m_Data.pop_back();
            return true;
        }
        return false;
    }
    
    template<typename T>
    void TArray<T>::SetNum(SizeType NewNum)
    {
        m_Data.resize(NewNum);
    }
    
    template<typename T>
    void TArray<T>::SetNum(SizeType NewNum, const T& Value)
    {
        SizeType OldNum = static_cast<SizeType>(m_Data.size());
        m_Data.resize(NewNum);
        for (SizeType i = OldNum; i < NewNum; ++i)
        {
            m_Data[i] = Value;
        }
    }
    
    template<typename T>
    typename TArray<T>::IndexType TArray<T>::Find(const T& Item) const
    {
        auto it = std::find(m_Data.begin(), m_Data.end(), Item);
        return it != m_Data.end() ? static_cast<IndexType>(it - m_Data.begin()) : -1;
    }
    
    template<typename T>
    bool TArray<T>::Contains(const T& Item) const
    {
        return std::find(m_Data.begin(), m_Data.end(), Item) != m_Data.end();
    }
    
    template<typename T>
    void TArray<T>::Sort()
    {
        std::sort(m_Data.begin(), m_Data.end());
    }
    
    template<typename T>
    template<typename Predicate>
    void TArray<T>::Sort(Predicate Pred)
    {
        std::sort(m_Data.begin(), m_Data.end(), Pred);
    }
    
    template<typename T>
    void TArray<T>::Reverse()
    {
        std::reverse(m_Data.begin(), m_Data.end());
    }

    // TMap implementation
    template<typename K, typename V>
    TArray<K> TMap<K, V>::GetKeys() const
    {
        TArray<K> Keys;
        for (const auto& pair : m_Data)
        {
            Keys.Add(pair.first);
        }
        return Keys;
    }
    
    template<typename K, typename V>
    TArray<V> TMap<K, V>::GetValues() const
    {
        TArray<V> Values;
        for (const auto& pair : m_Data)
        {
            Values.Add(pair.second);
        }
        return Values;
    }

    // TSet implementation
    template<typename T>
    TArray<T> TSet<T>::ToArray() const
    {
        TArray<T> Array;
        for (const T& item : m_Data)
        {
            Array.Add(item);
        }
        return Array;
    }
    
    template<typename T>
    TSet<T> TSet<T>::ToSet(const TArray<T>& Array)
    {
        TSet<T> Set;
        for (const T& item : Array)
        {
            Set.Add(item);
        }
        return Set;
    }
}
