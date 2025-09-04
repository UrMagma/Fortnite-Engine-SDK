/*
 * UE4 Utils - Self-contained utility functions
 * Vibe coder approved ✨
 */

#pragma once

#include "Types.h"
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>
#include <fstream>
#include <filesystem>

namespace UE4SDK
{
    /**
     * Utils - Your utility toolkit for UE4 development
     * Clean, simple, and powerful
     */
    class UE4SDK_API Utils
    {
    public:
        // Singleton access
        static Utils& Get();
        
        // Math utilities
        static float Lerp(float A, float B, float Alpha);
        static FVector LerpVector(const FVector& A, const FVector& B, float Alpha);
        static FRotator LerpRotator(const FRotator& A, const FRotator& B, float Alpha);
        static FLinearColor LerpColor(const FLinearColor& A, const FLinearColor& B, float Alpha);
        
        static float Clamp(float Value, float Min, float Max);
        static FVector ClampVector(const FVector& Value, const FVector& Min, const FVector& Max);
        static FRotator ClampRotator(const FRotator& Value, const FRotator& Min, const FRotator& Max);
        
        static float MapRange(float Value, float InMin, float InMax, float OutMin, float OutMax);
        static FVector MapRangeVector(const FVector& Value, const FVector& InMin, const FVector& InMax, const FVector& OutMin, const FVector& OutMax);
        
        static float SmoothStep(float Value);
        static float SmoothStep(float Value, float Edge0, float Edge1);
        
        static float RandomFloat(float Min = 0.0f, float Max = 1.0f);
        static int32 RandomInt(int32 Min, int32 Max);
        static FVector RandomVector(const FVector& Min, const FVector& Max);
        static FRotator RandomRotator();
        static FLinearColor RandomColor();
        
        static float Distance(const FVector& A, const FVector& B);
        static float Distance2D(const FVector& A, const FVector& B);
        static float DistanceSquared(const FVector& A, const FVector& B);
        static float DistanceSquared2D(const FVector& A, const FVector& B);
        
        static FVector Direction(const FVector& From, const FVector& To);
        static FVector Direction2D(const FVector& From, const FVector& To);
        static FRotator RotationFromDirection(const FVector& Direction);
        static FRotator LookAtRotation(const FVector& From, const FVector& To);
        
        static bool IsPointInSphere(const FVector& Point, const FVector& SphereCenter, float SphereRadius);
        static bool IsPointInBox(const FVector& Point, const FVector& BoxCenter, const FVector& BoxExtent);
        static bool IsPointInCapsule(const FVector& Point, const FVector& CapsuleCenter, float CapsuleRadius, float CapsuleHalfHeight);
        
        // String utilities
        static FString ToString(float Value, int32 DecimalPlaces = 2);
        static FString ToString(int32 Value);
        static FString ToString(bool Value);
        static FString ToString(const FVector& Value, int32 DecimalPlaces = 2);
        static FString ToString(const FRotator& Value, int32 DecimalPlaces = 2);
        static FString ToString(const FLinearColor& Value, int32 DecimalPlaces = 2);
        
        static float ToFloat(const FString& String);
        static int32 ToInt(const FString& String);
        static bool ToBool(const FString& String);
        static FVector ToVector(const FString& String);
        static FRotator ToRotator(const FString& String);
        static FLinearColor ToColor(const FString& String);
        
        static FString FormatString(const FString& Format, ...);
        static TArray<FString> SplitString(const FString& String, const FString& Delimiter);
        static FString JoinString(const TArray<FString>& Strings, const FString& Delimiter);
        static FString ReplaceString(const FString& String, const FString& From, const FString& To);
        static bool ContainsString(const FString& String, const FString& Substring);
        static bool StartsWith(const FString& String, const FString& Prefix);
        static bool EndsWith(const FString& String, const FString& Suffix);
        static FString ToUpper(const FString& String);
        static FString ToLower(const FString& String);
        static FString Trim(const FString& String);
        
        // Array utilities
        template<typename T>
        static TArray<T> ShuffleArray(const TArray<T>& Array);
        
        template<typename T>
        static T GetRandomElement(const TArray<T>& Array);
        
        template<typename T>
        static TArray<T> GetRandomElements(const TArray<T>& Array, int32 Count);
        
        template<typename T>
        static bool Contains(const TArray<T>& Array, const T& Element);
        
        template<typename T>
        static int32 FindIndex(const TArray<T>& Array, const T& Element);
        
        template<typename T>
        static TArray<T> Filter(const TArray<T>& Array, std::function<bool(const T&)> Predicate);
        
        template<typename T, typename U>
        static TArray<U> Map(const TArray<T>& Array, std::function<U(const T&)> Transform);
        
        // Map utilities
        template<typename K, typename V>
        static bool Contains(const TMap<K, V>& Map, const K& Key);
        
        template<typename K, typename V>
        static V GetValueOrDefault(const TMap<K, V>& Map, const K& Key, const V& DefaultValue);
        
        template<typename K, typename V>
        static TArray<K> GetKeys(const TMap<K, V>& Map);
        
        template<typename K, typename V>
        static TArray<V> GetValues(const TMap<K, V>& Map);
        
        // Set utilities
        template<typename T>
        static bool Contains(const TSet<T>& Set, const T& Element);
        
        template<typename T>
        static TArray<T> ToArray(const TSet<T>& Set);
        
        template<typename T>
        static TSet<T> ToSet(const TArray<T>& Array);
        
        // Time utilities
        static FDateTime GetCurrentTime();
        static FString GetCurrentTimeString();
        static float GetTimeSinceStart();
        static float GetDeltaTime();
        static void Sleep(float Seconds);
        static void SleepAsync(float Seconds, std::function<void()> Callback);
        
        // File utilities
        static bool FileExists(const FString& FilePath);
        static bool DirectoryExists(const FString& DirectoryPath);
        static bool CreateDirectory(const FString& DirectoryPath);
        static bool DeleteFile(const FString& FilePath);
        static bool DeleteDirectory(const FString& DirectoryPath);
        static TArray<FString> GetFilesInDirectory(const FString& DirectoryPath, const FString& Extension = TEXT(""));
        static TArray<FString> GetDirectoriesInDirectory(const FString& DirectoryPath);
        static FString GetFileName(const FString& FilePath);
        static FString GetFileNameWithoutExtension(const FString& FilePath);
        static FString GetFileExtension(const FString& FilePath);
        static FString GetDirectoryPath(const FString& FilePath);
        
        static bool SaveStringToFile(const FString& String, const FString& FilePath);
        static FString LoadStringFromFile(const FString& FilePath);
        static bool SaveBytesToFile(const TArray<uint8>& Bytes, const FString& FilePath);
        static TArray<uint8> LoadBytesFromFile(const FString& FilePath);
        
        // Logging utilities
        static void Log(const FString& Message, ELogVerbosity::Type Verbosity = ELogVerbosity::Log);
        static void LogWarning(const FString& Message);
        static void LogError(const FString& Message);
        static void LogInfo(const FString& Message);
        static void LogDebug(const FString& Message);
        
        // Debug utilities
        static void DrawDebugLine(const FVector& Start, const FVector& End, const FLinearColor& Color = FLinearColor::White, float Duration = 0.0f);
        static void DrawDebugSphere(const FVector& Center, float Radius, const FLinearColor& Color = FLinearColor::White, float Duration = 0.0f);
        static void DrawDebugBox(const FVector& Center, const FVector& Extent, const FLinearColor& Color = FLinearColor::White, float Duration = 0.0f);
        static void DrawDebugCapsule(const FVector& Center, float HalfHeight, float Radius, const FLinearColor& Color = FLinearColor::White, float Duration = 0.0f);
        static void DrawDebugString(const FVector& Location, const FString& Text, const FLinearColor& Color = FLinearColor::White, float Duration = 0.0f);
        static void DrawDebugArrow(const FVector& Start, const FVector& End, float ArrowSize, const FLinearColor& Color = FLinearColor::White, float Duration = 0.0f);
        
        // Performance utilities
        static void StartTimer(const FString& TimerName);
        static float StopTimer(const FString& TimerName);
        static float GetTimerElapsed(const FString& TimerName);
        static void ResetTimer(const FString& TimerName);
        static bool IsTimerRunning(const FString& TimerName);
        
        static void StartProfiler(const FString& ProfilerName);
        static void StopProfiler(const FString& ProfilerName);
        static void LogProfilerResults();
        
        // Memory utilities
        static void ForceGarbageCollection();
        static void TrimMemory();
        static int64 GetMemoryUsage();
        static int64 GetAvailableMemory();
        
        // Platform utilities
        static FString GetPlatformName();
        static FString GetEngineVersion();
        static FString GetBuildConfiguration();
        static bool IsEditor();
        static bool IsGame();
        static bool IsServer();
        static bool IsClient();
        static bool IsDedicatedServer();
        static bool IsListenServer();
        
        // Input utilities
        static bool IsKeyPressed(const FString& KeyName);
        static bool IsMouseButtonPressed(int32 ButtonIndex);
        static FVector2D GetMousePosition();
        static FVector2D GetMouseDelta();
        static float GetMouseWheelDelta();
        
        // Audio utilities
        static void PlaySound2D(USoundBase* Sound, float VolumeMultiplier = 1.0f, float PitchMultiplier = 1.0f);
        static void StopAllSounds();
        static void SetMasterVolume(float Volume);
        static float GetMasterVolume();
        static void SetSFXVolume(float Volume);
        static float GetSFXVolume();
        static void SetMusicVolume(float Volume);
        static float GetMusicVolume();
        
        // Rendering utilities
        static void SetViewMode(EViewModeIndex ViewMode);
        static void SetShowFlags(bool bShowFlags);
        static void SetPostProcessSettings(const FString& SettingName, float Value);
        static void SetScreenResolution(int32 Width, int32 Height);
        static void SetFullscreen(bool bFullscreen);
        static void SetVSync(bool bVSync);
        
        // Networking utilities
        static bool IsConnected();
        static FString GetServerIP();
        static int32 GetServerPort();
        static FString GetPlayerName();
        static int32 GetPlayerID();
        static bool IsHost();
        static bool IsClient();
        
        // Console utilities
        static void ExecuteConsoleCommand(const FString& Command);
        static FString GetConsoleVariable(const FString& VariableName);
        static void SetConsoleVariable(const FString& VariableName, const FString& Value);
        static void SetConsoleVariable(const FString& VariableName, float Value);
        static void SetConsoleVariable(const FString& VariableName, int32 Value);
        static void SetConsoleVariable(const FString& VariableName, bool Value);
        
        // Event utilities
        static void BindEvent(const FString& EventName, std::function<void()> Callback);
        static void UnbindEvent(const FString& EventName);
        static void TriggerEvent(const FString& EventName);
        static bool IsEventBound(const FString& EventName);
        
        // Async utilities
        static void RunAsync(std::function<void()> Function);
        static void RunAsyncWithCallback(std::function<void()> Function, std::function<void()> Callback);
        static void RunOnGameThread(std::function<void()> Function);
        static void RunOnRenderThread(std::function<void()> Function);
        
        // Validation utilities
        static bool IsValidObject(UObject* Object);
        static bool IsValidActor(AActor* Actor);
        static bool IsValidComponent(UActorComponent* Component);
        static bool IsValidWorld(UWorld* World);
        static bool IsValidPlayerController(APlayerController* PlayerController);
        static bool IsValidPawn(APawn* Pawn);
        
        // Conversion utilities
        static FVector ToUE4Vector(const FVector& Vector);
        static FRotator ToUE4Rotator(const FRotator& Rotator);
        static FTransform ToUE4Transform(const FTransform& Transform);
        static FLinearColor ToUE4Color(const FLinearColor& Color);
        
        // Helper functions
        static FString GetRandomString(int32 Length = 10);
        static FString GetRandomName();
        static FString GetRandomEmail();
        static FString GetRandomIP();
        static FString GetRandomMAC();
        
        static int32 GetRandomPort();
        static FString GetRandomUUID();
        static FString GetRandomGUID();
        
        // Color utilities
        static FLinearColor HSVToRGB(float H, float S, float V);
        static FLinearColor RGBToHSV(const FLinearColor& RGB);
        static FLinearColor HexToColor(const FString& Hex);
        static FString ColorToHex(const FLinearColor& Color);
        
        // Angle utilities
        static float DegreesToRadians(float Degrees);
        static float RadiansToDegrees(float Radians);
        static float NormalizeAngle(float Angle);
        static float AngleDifference(float Angle1, float Angle2);
        
        // Interpolation utilities
        static float EaseIn(float Value);
        static float EaseOut(float Value);
        static float EaseInOut(float Value);
        static float BounceIn(float Value);
        static float BounceOut(float Value);
        static float BounceInOut(float Value);
        static float ElasticIn(float Value);
        static float ElasticOut(float Value);
        static float ElasticInOut(float Value);
        
    private:
        Utils() = default;
        ~Utils() = default;
        
        Utils(const Utils&) = delete;
        Utils& operator=(const Utils&) = delete;
        
        // Internal data
        TMap<FString, float> m_Timers;
        TMap<FString, std::function<void()>> m_Events;
        std::chrono::steady_clock::time_point m_StartTime;
        std::mt19937 m_RandomGenerator;
        
        // Internal functions
        void Initialize();
        void Shutdown();
        
        friend class SDK;
    };
}
