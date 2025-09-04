/*
 * UE4 Utils Implementation - Self-contained utility functions implementation
 * Vibe coder approved ✨
 */

#include "Utils.h"
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <filesystem>
#include <cstdarg>

namespace UE4SDK
{
    Utils* Utils::s_Instance = nullptr;

    Utils& Utils::Get()
    {
        if (!s_Instance)
        {
            s_Instance = new Utils();
        }
        return *s_Instance;
    }

    void Utils::Initialize()
    {
        m_StartTime = std::chrono::steady_clock::now();
        m_RandomGenerator.seed(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
    }

    void Utils::Shutdown()
    {
        m_Timers.clear();
        m_Events.clear();
    }

    // Math utilities
    float Utils::Lerp(float A, float B, float Alpha)
    {
        return A + (B - A) * Alpha;
    }

    FVector Utils::LerpVector(const FVector& A, const FVector& B, float Alpha)
    {
        return FVector(
            Lerp(A.X, B.X, Alpha),
            Lerp(A.Y, B.Y, Alpha),
            Lerp(A.Z, B.Z, Alpha)
        );
    }

    FRotator Utils::LerpRotator(const FRotator& A, const FRotator& B, float Alpha)
    {
        return FRotator(
            Lerp(A.Pitch, B.Pitch, Alpha),
            Lerp(A.Yaw, B.Yaw, Alpha),
            Lerp(A.Roll, B.Roll, Alpha)
        );
    }

    FLinearColor Utils::LerpColor(const FLinearColor& A, const FLinearColor& B, float Alpha)
    {
        return FLinearColor(
            Lerp(A.R, B.R, Alpha),
            Lerp(A.G, B.G, Alpha),
            Lerp(A.B, B.B, Alpha),
            Lerp(A.A, B.A, Alpha)
        );
    }

    float Utils::Clamp(float Value, float Min, float Max)
    {
        return std::max(Min, std::min(Max, Value));
    }

    FVector Utils::ClampVector(const FVector& Value, const FVector& Min, const FVector& Max)
    {
        return FVector(
            Clamp(Value.X, Min.X, Max.X),
            Clamp(Value.Y, Min.Y, Max.Y),
            Clamp(Value.Z, Min.Z, Max.Z)
        );
    }

    FRotator Utils::ClampRotator(const FRotator& Value, const FRotator& Min, const FRotator& Max)
    {
        return FRotator(
            Clamp(Value.Pitch, Min.Pitch, Max.Pitch),
            Clamp(Value.Yaw, Min.Yaw, Max.Yaw),
            Clamp(Value.Roll, Min.Roll, Max.Roll)
        );
    }

    float Utils::MapRange(float Value, float InMin, float InMax, float OutMin, float OutMax)
    {
        return OutMin + (Value - InMin) * (OutMax - OutMin) / (InMax - InMin);
    }

    FVector Utils::MapRangeVector(const FVector& Value, const FVector& InMin, const FVector& InMax, const FVector& OutMin, const FVector& OutMax)
    {
        return FVector(
            MapRange(Value.X, InMin.X, InMax.X, OutMin.X, OutMax.X),
            MapRange(Value.Y, InMin.Y, InMax.Y, OutMin.Y, OutMax.Y),
            MapRange(Value.Z, InMin.Z, InMax.Z, OutMin.Z, OutMax.Z)
        );
    }

    float Utils::SmoothStep(float Value)
    {
        return Value * Value * (3.0f - 2.0f * Value);
    }

    float Utils::SmoothStep(float Value, float Edge0, float Edge1)
    {
        float t = Clamp((Value - Edge0) / (Edge1 - Edge0), 0.0f, 1.0f);
        return t * t * (3.0f - 2.0f * t);
    }

    float Utils::RandomFloat(float Min, float Max)
    {
        std::uniform_real_distribution<float> dist(Min, Max);
        return dist(m_RandomGenerator);
    }

    int32 Utils::RandomInt(int32 Min, int32 Max)
    {
        std::uniform_int_distribution<int32> dist(Min, Max);
        return dist(m_RandomGenerator);
    }

    FVector Utils::RandomVector(const FVector& Min, const FVector& Max)
    {
        return FVector(
            RandomFloat(Min.X, Max.X),
            RandomFloat(Min.Y, Max.Y),
            RandomFloat(Min.Z, Max.Z)
        );
    }

    FRotator Utils::RandomRotator()
    {
        return FRotator(
            RandomFloat(-180.0f, 180.0f),
            RandomFloat(-180.0f, 180.0f),
            RandomFloat(-180.0f, 180.0f)
        );
    }

    FLinearColor Utils::RandomColor()
    {
        return FLinearColor(
            RandomFloat(0.0f, 1.0f),
            RandomFloat(0.0f, 1.0f),
            RandomFloat(0.0f, 1.0f),
            1.0f
        );
    }

    float Utils::Distance(const FVector& A, const FVector& B)
    {
        return (A - B).Size();
    }

    float Utils::Distance2D(const FVector& A, const FVector& B)
    {
        return (A - B).Size2D();
    }

    float Utils::DistanceSquared(const FVector& A, const FVector& B)
    {
        return (A - B).SizeSquared();
    }

    float Utils::DistanceSquared2D(const FVector& A, const FVector& B)
    {
        return (A - B).SizeSquared2D();
    }

    FVector Utils::Direction(const FVector& From, const FVector& To)
    {
        return (To - From).GetSafeNormal();
    }

    FVector Utils::Direction2D(const FVector& From, const FVector& To)
    {
        FVector Dir = To - From;
        Dir.Z = 0.0f;
        return Dir.GetSafeNormal();
    }

    FRotator Utils::RotationFromDirection(const FVector& Direction)
    {
        FVector NormalizedDir = Direction.GetSafeNormal();
        float Pitch = std::asin(NormalizedDir.Z) * (180.0f / 3.14159265359f);
        float Yaw = std::atan2(NormalizedDir.Y, NormalizedDir.X) * (180.0f / 3.14159265359f);
        return FRotator(Pitch, Yaw, 0.0f);
    }

    FRotator Utils::LookAtRotation(const FVector& From, const FVector& To)
    {
        return RotationFromDirection(Direction(From, To));
    }

    bool Utils::IsPointInSphere(const FVector& Point, const FVector& SphereCenter, float SphereRadius)
    {
        return DistanceSquared(Point, SphereCenter) <= SphereRadius * SphereRadius;
    }

    bool Utils::IsPointInBox(const FVector& Point, const FVector& BoxCenter, const FVector& BoxExtent)
    {
        return Point.X >= BoxCenter.X - BoxExtent.X && Point.X <= BoxCenter.X + BoxExtent.X &&
               Point.Y >= BoxCenter.Y - BoxExtent.Y && Point.Y <= BoxCenter.Y + BoxExtent.Y &&
               Point.Z >= BoxCenter.Z - BoxExtent.Z && Point.Z <= BoxCenter.Z + BoxExtent.Z;
    }

    bool Utils::IsPointInCapsule(const FVector& Point, const FVector& CapsuleCenter, float CapsuleRadius, float CapsuleHalfHeight)
    {
        FVector PointToCenter = Point - CapsuleCenter;
        float DistanceFromAxis = FVector(PointToCenter.X, PointToCenter.Y, 0.0f).Size();
        float DistanceFromCenter = std::abs(PointToCenter.Z);
        
        return DistanceFromAxis <= CapsuleRadius && DistanceFromCenter <= CapsuleHalfHeight;
    }

    // String utilities
    FString Utils::ToString(float Value, int32 DecimalPlaces)
    {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(DecimalPlaces) << Value;
        return FString(oss.str());
    }

    FString Utils::ToString(int32 Value)
    {
        return FString::FromInt(Value);
    }

    FString Utils::ToString(bool Value)
    {
        return FString::FromBool(Value);
    }

    FString Utils::ToString(const FVector& Value, int32 DecimalPlaces)
    {
        return Value.ToString();
    }

    FString Utils::ToString(const FRotator& Value, int32 DecimalPlaces)
    {
        return Value.ToString();
    }

    FString Utils::ToString(const FLinearColor& Value, int32 DecimalPlaces)
    {
        return Value.ToString();
    }

    float Utils::ToFloat(const FString& String)
    {
        return String.ToFloat();
    }

    int32 Utils::ToInt(const FString& String)
    {
        return String.ToInt();
    }

    bool Utils::ToBool(const FString& String)
    {
        return String.ToBool();
    }

    FVector Utils::ToVector(const FString& String)
    {
        auto Parts = String.Split(TEXT(","));
        if (Parts.size() >= 3)
        {
            return FVector(
                FString(Parts[0]).ToFloat(),
                FString(Parts[1]).ToFloat(),
                FString(Parts[2]).ToFloat()
            );
        }
        return FVector::ZeroVector;
    }

    FRotator Utils::ToRotator(const FString& String)
    {
        auto Parts = String.Split(TEXT(","));
        if (Parts.size() >= 3)
        {
            return FRotator(
                FString(Parts[0]).ToFloat(),
                FString(Parts[1]).ToFloat(),
                FString(Parts[2]).ToFloat()
            );
        }
        return FRotator::ZeroRotator;
    }

    FLinearColor Utils::ToColor(const FString& String)
    {
        auto Parts = String.Split(TEXT(","));
        if (Parts.size() >= 3)
        {
            float A = Parts.size() >= 4 ? FString(Parts[3]).ToFloat() : 1.0f;
            return FLinearColor(
                FString(Parts[0]).ToFloat(),
                FString(Parts[1]).ToFloat(),
                FString(Parts[2]).ToFloat(),
                A
            );
        }
        return FLinearColor::White;
    }

    FString Utils::FormatString(const FString& Format, ...)
    {
        va_list args;
        va_start(args, Format);
        
        char buffer[1024];
        vsnprintf(buffer, sizeof(buffer), Format.c_str(), args);
        
        va_end(args);
        
        return FString(buffer);
    }

    TArray<FString> Utils::SplitString(const FString& String, const FString& Delimiter)
    {
        auto parts = String.Split(Delimiter);
        TArray<FString> result;
        for (const auto& part : parts)
        {
            result.Add(part);
        }
        return result;
    }

    FString Utils::JoinString(const TArray<FString>& Strings, const FString& Delimiter)
    {
        FString result;
        for (int32 i = 0; i < Strings.Num(); ++i)
        {
            if (i > 0) result += Delimiter;
            result += Strings[i];
        }
        return result;
    }

    FString Utils::ReplaceString(const FString& String, const FString& From, const FString& To)
    {
        return String.Replace(From, To);
    }

    bool Utils::ContainsString(const FString& String, const FString& Substring)
    {
        return String.Contains(Substring);
    }

    bool Utils::StartsWith(const FString& String, const FString& Prefix)
    {
        return String.StartsWith(Prefix);
    }

    bool Utils::EndsWith(const FString& String, const FString& Suffix)
    {
        return String.EndsWith(Suffix);
    }

    FString Utils::ToUpper(const FString& String)
    {
        return String.ToUpper();
    }

    FString Utils::ToLower(const FString& String)
    {
        return String.ToLower();
    }

    FString Utils::Trim(const FString& String)
    {
        return String.Trim();
    }

    // Time utilities
    FDateTime Utils::GetCurrentTime()
    {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        return FDateTime(static_cast<int64>(time_t));
    }

    FString Utils::GetCurrentTimeString()
    {
        auto now = std::chrono::system_clock::now();
        auto time_t = std::chrono::system_clock::to_time_t(now);
        auto tm = *std::localtime(&time_t);
        
        char buffer[64];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
        return FString(buffer);
    }

    float Utils::GetTimeSinceStart()
    {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_StartTime);
        return duration.count() / 1000.0f;
    }

    float Utils::GetDeltaTime()
    {
        static auto lastTime = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(now - lastTime);
        lastTime = now;
        return duration.count() / 1000000.0f;
    }

    void Utils::Sleep(float Seconds)
    {
        std::this_thread::sleep_for(std::chrono::duration<float>(Seconds));
    }

    void Utils::SleepAsync(float Seconds, std::function<void()> Callback)
    {
        std::thread([Seconds, Callback]() {
            std::this_thread::sleep_for(std::chrono::duration<float>(Seconds));
            Callback();
        }).detach();
    }

    // File utilities
    bool Utils::FileExists(const FString& FilePath)
    {
        return std::filesystem::exists(FilePath.c_str());
    }

    bool Utils::DirectoryExists(const FString& DirectoryPath)
    {
        return std::filesystem::exists(DirectoryPath.c_str()) && std::filesystem::is_directory(DirectoryPath.c_str());
    }

    bool Utils::CreateDirectory(const FString& DirectoryPath)
    {
        return std::filesystem::create_directories(DirectoryPath.c_str());
    }

    bool Utils::DeleteFile(const FString& FilePath)
    {
        return std::filesystem::remove(FilePath.c_str());
    }

    bool Utils::DeleteDirectory(const FString& DirectoryPath)
    {
        return std::filesystem::remove_all(DirectoryPath.c_str()) > 0;
    }

    TArray<FString> Utils::GetFilesInDirectory(const FString& DirectoryPath, const FString& Extension)
    {
        TArray<FString> Files;
        try
        {
            for (const auto& entry : std::filesystem::directory_iterator(DirectoryPath.c_str()))
            {
                if (entry.is_regular_file())
                {
                    FString FileName = FString(entry.path().filename().string().c_str());
                    if (Extension.IsEmpty() || FileName.EndsWith(Extension))
                    {
                        Files.Add(FileName);
                    }
                }
            }
        }
        catch (...)
        {
            // Directory doesn't exist or can't be accessed
        }
        return Files;
    }

    TArray<FString> Utils::GetDirectoriesInDirectory(const FString& DirectoryPath)
    {
        TArray<FString> Directories;
        try
        {
            for (const auto& entry : std::filesystem::directory_iterator(DirectoryPath.c_str()))
            {
                if (entry.is_directory())
                {
                    Directories.Add(FString(entry.path().filename().string().c_str()));
                }
            }
        }
        catch (...)
        {
            // Directory doesn't exist or can't be accessed
        }
        return Directories;
    }

    FString Utils::GetFileName(const FString& FilePath)
    {
        std::filesystem::path path(FilePath.c_str());
        return FString(path.filename().string().c_str());
    }

    FString Utils::GetFileNameWithoutExtension(const FString& FilePath)
    {
        std::filesystem::path path(FilePath.c_str());
        return FString(path.stem().string().c_str());
    }

    FString Utils::GetFileExtension(const FString& FilePath)
    {
        std::filesystem::path path(FilePath.c_str());
        return FString(path.extension().string().c_str());
    }

    FString Utils::GetDirectoryPath(const FString& FilePath)
    {
        std::filesystem::path path(FilePath.c_str());
        return FString(path.parent_path().string().c_str());
    }

    bool Utils::SaveStringToFile(const FString& String, const FString& FilePath)
    {
        try
        {
            std::ofstream file(FilePath.c_str());
            if (file.is_open())
            {
                file << String.c_str();
                file.close();
                return true;
            }
        }
        catch (...)
        {
            // Error writing file
        }
        return false;
    }

    FString Utils::LoadStringFromFile(const FString& FilePath)
    {
        try
        {
            std::ifstream file(FilePath.c_str());
            if (file.is_open())
            {
                std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
                file.close();
                return FString(content.c_str());
            }
        }
        catch (...)
        {
            // Error reading file
        }
        return FString();
    }

    bool Utils::SaveBytesToFile(const TArray<uint8>& Bytes, const FString& FilePath)
    {
        try
        {
            std::ofstream file(FilePath.c_str(), std::ios::binary);
            if (file.is_open())
            {
                file.write(reinterpret_cast<const char*>(Bytes.GetData()), Bytes.Num());
                file.close();
                return true;
            }
        }
        catch (...)
        {
            // Error writing file
        }
        return false;
    }

    TArray<uint8> Utils::LoadBytesFromFile(const FString& FilePath)
    {
        TArray<uint8> Bytes;
        try
        {
            std::ifstream file(FilePath.c_str(), std::ios::binary);
            if (file.is_open())
            {
                file.seekg(0, std::ios::end);
                size_t size = file.tellg();
                file.seekg(0, std::ios::beg);
                
                Bytes.SetNum(static_cast<int32>(size));
                file.read(reinterpret_cast<char*>(Bytes.GetData()), size);
                file.close();
            }
        }
        catch (...)
        {
            // Error reading file
        }
        return Bytes;
    }

    // Logging utilities
    void Utils::Log(const FString& Message, ELogVerbosity::Type Verbosity)
    {
        const char* VerbosityName = "Log";
        switch (Verbosity)
        {
            case ELogVerbosity::Fatal: VerbosityName = "Fatal"; break;
            case ELogVerbosity::Error: VerbosityName = "Error"; break;
            case ELogVerbosity::Warning: VerbosityName = "Warning"; break;
            case ELogVerbosity::Display: VerbosityName = "Display"; break;
            case ELogVerbosity::Log: VerbosityName = "Log"; break;
            case ELogVerbosity::Verbose: VerbosityName = "Verbose"; break;
            case ELogVerbosity::VeryVerbose: VerbosityName = "VeryVerbose"; break;
        }
        
        printf("[%s] %s\n", VerbosityName, Message.c_str());
    }

    void Utils::LogWarning(const FString& Message)
    {
        Log(Message, ELogVerbosity::Warning);
    }

    void Utils::LogError(const FString& Message)
    {
        Log(Message, ELogVerbosity::Error);
    }

    void Utils::LogInfo(const FString& Message)
    {
        Log(Message, ELogVerbosity::Log);
    }

    void Utils::LogDebug(const FString& Message)
    {
        Log(Message, ELogVerbosity::Verbose);
    }

    // Performance utilities
    void Utils::StartTimer(const FString& TimerName)
    {
        m_Timers.Add(TimerName, GetTimeSinceStart());
    }

    float Utils::StopTimer(const FString& TimerName)
    {
        float* StartTime = m_Timers.Find(TimerName);
        if (StartTime)
        {
            float Elapsed = GetTimeSinceStart() - *StartTime;
            m_Timers.Remove(TimerName);
            return Elapsed;
        }
        return 0.0f;
    }

    float Utils::GetTimerElapsed(const FString& TimerName)
    {
        float* StartTime = m_Timers.Find(TimerName);
        if (StartTime)
        {
            return GetTimeSinceStart() - *StartTime;
        }
        return 0.0f;
    }

    void Utils::ResetTimer(const FString& TimerName)
    {
        m_Timers.Add(TimerName, GetTimeSinceStart());
    }

    bool Utils::IsTimerRunning(const FString& TimerName)
    {
        return m_Timers.Contains(TimerName);
    }

    // Platform utilities
    FString Utils::GetPlatformName()
    {
#ifdef UE4_PLATFORM_WINDOWS
        return TEXT("Windows");
#elif defined(UE4_PLATFORM_MAC)
        return TEXT("Mac");
#elif defined(UE4_PLATFORM_LINUX)
        return TEXT("Linux");
#else
        return TEXT("Unknown");
#endif
    }

    FString Utils::GetEngineVersion()
    {
        return TEXT(UE4_SDK_VERSION);
    }

    FString Utils::GetBuildConfiguration()
    {
#ifdef _DEBUG
        return TEXT("Debug");
#else
        return TEXT("Release");
#endif
    }

    bool Utils::IsEditor()
    {
        return false; // This is a runtime SDK
    }

    bool Utils::IsGame()
    {
        return true;
    }

    bool Utils::IsServer()
    {
        return false; // Default to client
    }

    bool Utils::IsClient()
    {
        return true; // Default to client
    }

    bool Utils::IsDedicatedServer()
    {
        return false;
    }

    bool Utils::IsListenServer()
    {
        return false;
    }

    // Event utilities
    void Utils::BindEvent(const FString& EventName, std::function<void()> Callback)
    {
        m_Events.Add(EventName, Callback);
    }

    void Utils::UnbindEvent(const FString& EventName)
    {
        m_Events.Remove(EventName);
    }

    void Utils::TriggerEvent(const FString& EventName)
    {
        auto* Callback = m_Events.Find(EventName);
        if (Callback)
        {
            (*Callback)();
        }
    }

    bool Utils::IsEventBound(const FString& EventName)
    {
        return m_Events.Contains(EventName);
    }

    // Helper functions
    FString Utils::GetRandomString(int32 Length)
    {
        const char* chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        FString result;
        for (int32 i = 0; i < Length; ++i)
        {
            result += chars[RandomInt(0, 61)];
        }
        return result;
    }

    FString Utils::GetRandomName()
    {
        TArray<FString> FirstNames = {TEXT("Alex"), TEXT("Jordan"), TEXT("Casey"), TEXT("Taylor"), TEXT("Morgan"), TEXT("Riley"), TEXT("Avery"), TEXT("Quinn")};
        TArray<FString> LastNames = {TEXT("Smith"), TEXT("Johnson"), TEXT("Williams"), TEXT("Brown"), TEXT("Jones"), TEXT("Garcia"), TEXT("Miller"), TEXT("Davis")};
        
        return FirstNames[RandomInt(0, FirstNames.Num() - 1)] + TEXT(" ") + LastNames[RandomInt(0, LastNames.Num() - 1)];
    }

    FString Utils::GetRandomEmail()
    {
        TArray<FString> Domains = {TEXT("gmail.com"), TEXT("yahoo.com"), TEXT("hotmail.com"), TEXT("outlook.com")};
        return GetRandomString(8) + TEXT("@") + Domains[RandomInt(0, Domains.Num() - 1)];
    }

    FString Utils::GetRandomIP()
    {
        return FString::FromInt(RandomInt(1, 255)) + TEXT(".") +
               FString::FromInt(RandomInt(1, 255)) + TEXT(".") +
               FString::FromInt(RandomInt(1, 255)) + TEXT(".") +
               FString::FromInt(RandomInt(1, 255));
    }

    FString Utils::GetRandomMAC()
    {
        FString result;
        for (int32 i = 0; i < 6; ++i)
        {
            if (i > 0) result += TEXT(":");
            result += FString::FromInt(RandomInt(0, 255), 16);
        }
        return result;
    }

    int32 Utils::GetRandomPort()
    {
        return RandomInt(1024, 65535);
    }

    FString Utils::GetRandomUUID()
    {
        return GetRandomString(8) + TEXT("-") + GetRandomString(4) + TEXT("-") + GetRandomString(4) + TEXT("-") + GetRandomString(4) + TEXT("-") + GetRandomString(12);
    }

    FString Utils::GetRandomGUID()
    {
        return TEXT("{") + GetRandomUUID() + TEXT("}");
    }

    // Color utilities
    FLinearColor Utils::HSVToRGB(float H, float S, float V)
    {
        float C = V * S;
        float X = C * (1.0f - std::abs(std::fmod(H / 60.0f, 2.0f) - 1.0f));
        float m = V - C;
        
        float R, G, B;
        if (H >= 0 && H < 60)
        {
            R = C; G = X; B = 0;
        }
        else if (H >= 60 && H < 120)
        {
            R = X; G = C; B = 0;
        }
        else if (H >= 120 && H < 180)
        {
            R = 0; G = C; B = X;
        }
        else if (H >= 180 && H < 240)
        {
            R = 0; G = X; B = C;
        }
        else if (H >= 240 && H < 300)
        {
            R = X; G = 0; B = C;
        }
        else
        {
            R = C; G = 0; B = X;
        }
        
        return FLinearColor(R + m, G + m, B + m, 1.0f);
    }

    FLinearColor Utils::RGBToHSV(const FLinearColor& RGB)
    {
        float R = RGB.R, G = RGB.G, B = RGB.B;
        float Max = std::max({R, G, B});
        float Min = std::min({R, G, B});
        float Delta = Max - Min;
        
        float H = 0.0f;
        if (Delta != 0)
        {
            if (Max == R)
                H = 60.0f * std::fmod((G - B) / Delta, 6.0f);
            else if (Max == G)
                H = 60.0f * ((B - R) / Delta + 2.0f);
            else
                H = 60.0f * ((R - G) / Delta + 4.0f);
        }
        
        float S = Max == 0 ? 0 : Delta / Max;
        float V = Max;
        
        return FLinearColor(H, S, V, RGB.A);
    }

    FLinearColor Utils::HexToColor(const FString& Hex)
    {
        FString CleanHex = Hex.Replace(TEXT("#"), TEXT(""));
        if (CleanHex.Len() == 6)
        {
            uint32 Color = std::stoul(CleanHex.c_str(), nullptr, 16);
            return FLinearColor(
                ((Color >> 16) & 0xFF) / 255.0f,
                ((Color >> 8) & 0xFF) / 255.0f,
                (Color & 0xFF) / 255.0f,
                1.0f
            );
        }
        return FLinearColor::White;
    }

    FString Utils::ColorToHex(const FLinearColor& Color)
    {
        uint32 R = static_cast<uint32>(Color.R * 255) & 0xFF;
        uint32 G = static_cast<uint32>(Color.G * 255) & 0xFF;
        uint32 B = static_cast<uint32>(Color.B * 255) & 0xFF;
        
        char buffer[8];
        snprintf(buffer, sizeof(buffer), "#%02X%02X%02X", R, G, B);
        return FString(buffer);
    }

    // Angle utilities
    float Utils::DegreesToRadians(float Degrees)
    {
        return Degrees * (3.14159265359f / 180.0f);
    }

    float Utils::RadiansToDegrees(float Radians)
    {
        return Radians * (180.0f / 3.14159265359f);
    }

    float Utils::NormalizeAngle(float Angle)
    {
        while (Angle < 0) Angle += 360.0f;
        while (Angle >= 360.0f) Angle -= 360.0f;
        return Angle;
    }

    float Utils::AngleDifference(float Angle1, float Angle2)
    {
        float Diff = Angle2 - Angle1;
        while (Diff > 180.0f) Diff -= 360.0f;
        while (Diff < -180.0f) Diff += 360.0f;
        return Diff;
    }

    // Interpolation utilities
    float Utils::EaseIn(float Value)
    {
        return Value * Value;
    }

    float Utils::EaseOut(float Value)
    {
        return 1.0f - (1.0f - Value) * (1.0f - Value);
    }

    float Utils::EaseInOut(float Value)
    {
        return Value < 0.5f ? 2.0f * Value * Value : 1.0f - 2.0f * (1.0f - Value) * (1.0f - Value);
    }

    float Utils::BounceIn(float Value)
    {
        return 1.0f - BounceOut(1.0f - Value);
    }

    float Utils::BounceOut(float Value)
    {
        if (Value < 1.0f / 2.75f)
        {
            return 7.5625f * Value * Value;
        }
        else if (Value < 2.0f / 2.75f)
        {
            return 7.5625f * (Value -= 1.5f / 2.75f) * Value + 0.75f;
        }
        else if (Value < 2.5f / 2.75f)
        {
            return 7.5625f * (Value -= 2.25f / 2.75f) * Value + 0.9375f;
        }
        else
        {
            return 7.5625f * (Value -= 2.625f / 2.75f) * Value + 0.984375f;
        }
    }

    float Utils::BounceInOut(float Value)
    {
        return Value < 0.5f ? BounceIn(Value * 2.0f) * 0.5f : BounceOut(Value * 2.0f - 1.0f) * 0.5f + 0.5f;
    }

    float Utils::ElasticIn(float Value)
    {
        if (Value == 0.0f || Value == 1.0f) return Value;
        return -std::pow(2.0f, 10.0f * (Value - 1.0f)) * std::sin((Value - 1.0f - 0.1f) * (2.0f * 3.14159265359f) / 0.4f);
    }

    float Utils::ElasticOut(float Value)
    {
        if (Value == 0.0f || Value == 1.0f) return Value;
        return std::pow(2.0f, -10.0f * Value) * std::sin((Value - 0.1f) * (2.0f * 3.14159265359f) / 0.4f) + 1.0f;
    }

    float Utils::ElasticInOut(float Value)
    {
        if (Value == 0.0f || Value == 1.0f) return Value;
        if (Value < 0.5f)
        {
            return -0.5f * std::pow(2.0f, 10.0f * (Value * 2.0f - 1.0f)) * std::sin((Value * 2.0f - 1.0f - 0.1f) * (2.0f * 3.14159265359f) / 0.4f);
        }
        else
        {
            return 0.5f * std::pow(2.0f, -10.0f * (Value * 2.0f - 1.0f)) * std::sin((Value * 2.0f - 1.0f - 0.1f) * (2.0f * 3.14159265359f) / 0.4f) + 1.0f;
        }
    }
}
