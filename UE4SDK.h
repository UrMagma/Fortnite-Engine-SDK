/*
 * UE4 SDK - Self-Contained Unreal Engine 4.26 SDK
 * Clean, vibe coder style interface with embedded UE4 types
 * 
 * Drop this entire folder into any project and it just works! ✨
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <functional>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <cstdint>
#include <type_traits>

// Platform detection
#ifdef _WIN32
    #define UE4_PLATFORM_WINDOWS 1
#elif defined(__APPLE__)
    #define UE4_PLATFORM_MAC 1
#elif defined(__linux__)
    #define UE4_PLATFORM_LINUX 1
#endif

// UE4 SDK Version
#define UE4_SDK_VERSION "4.26.0"
#define UE4_SDK_NAMESPACE UE4SDK

// Forward declarations
namespace UE4SDK
{
    // Core types
    class FString;
    class FName;
    class FVector;
    class FRotator;
    class FTransform;
    class FQuat;
    class FLinearColor;
    class FColor;
    class FBox;
    class FSphere;
    class FPlane;
    class FMatrix;
    class FIntPoint;
    class FIntVector;
    class FVector2D;
    class FVector4;
    class FDateTime;
    class FTimespan;
    class FGuid;
    
    // Containers
    template<typename T> class TArray;
    template<typename K, typename V> class TMap;
    template<typename T> class TSet;
    template<typename T, int32 N> class TStaticArray;
    
    // Objects
    class UObject;
    class UClass;
    class UWorld;
    class AActor;
    class APawn;
    class APlayerController;
    class AGameMode;
    class UActorComponent;
    class USceneComponent;
    class UStaticMeshComponent;
    class USkeletalMeshComponent;
    class UCameraComponent;
    class ULightComponent;
    class UAudioComponent;
    class UInputComponent;
    
    // AI System
    class AIController;
    class Blackboard;
    class BehaviorTree;
    class BehaviorTreeComponent;
    class BlackboardComponent;
    class AITask;
    class AIDecorator;
    class AIService;
    class AISystem;
    
    // Navigation System
    class NavMesh;
    class NavPath;
    class NavQueryFilter;
    class NavigationSystem;
    class NavigationSystemManager;
    
    // Replication System
    class ReplicationInfo;
    class ReplicationManager;
    class NetworkManager;
    class ServerReplication;
    class ReplicationSystem;
    
    // Assets
    class UStaticMesh;
    class USkeletalMesh;
    class UTexture2D;
    class USoundBase;
    class UMaterialInterface;
    class UBlueprint;
    class UAnimSequence;
    
    // Engine
    class UEngine;
    class FHitResult;
    class FDebugDrawInfo;
    
    // Enums
    enum class ELogVerbosity : uint8;
    enum class ECollisionChannel : uint8;
    enum class ECollisionResponse : uint8;
    enum class EViewModeIndex : uint8;
    enum class EInputEvent : uint8;
    enum class ENetRole : uint8;
    enum class EComponentMobility : uint8;
    enum class ECameraProjectionMode : uint8;
    enum class EControllerHand : uint8;
    enum class EEndPlayReason : uint8;
    enum class EAutoReceiveInput : uint8;
    enum class EAutoPossessAI : uint8;
    enum class ETravelType : uint8;
    enum class EMouseLockMode : uint8;
    enum class EViewTargetBlendFunction : uint8;
    enum class EObjectTypeQuery : uint8;
}

// Main SDK entry point
#define UE4_SDK UE4SDK::SDK::Get()
#define UE4_ENGINE UE4_SDK.GetEngine()
#define UE4_WORLD UE4_SDK.GetWorld()
#define UE4_UTILS UE4_SDK.GetUtils()

// Logging macros
#define UE_LOG(Category, Verbosity, Format, ...) \
    UE4SDK::Utils::Log(UE4SDK::Utils::FormatString(Format, ##__VA_ARGS__), UE4SDK::ELogVerbosity::Verbosity)

#define UE_LOG_VERBOSE(Format, ...) UE_LOG(LogUE4SDK, Verbose, Format, ##__VA_ARGS__)
#define UE_LOG_VERY_VERBOSE(Format, ...) UE_LOG(LogUE4SDK, VeryVerbose, Format, ##__VA_ARGS__)
#define UE_LOG_LOG(Format, ...) UE_LOG(LogUE4SDK, Log, Format, ##__VA_ARGS__)
#define UE_LOG_WARNING(Format, ...) UE_LOG(LogUE4SDK, Warning, Format, ##__VA_ARGS__)
#define UE_LOG_ERROR(Format, ...) UE_LOG(LogUE4SDK, Error, Format, ##__VA_ARGS__)
#define UE_LOG_FATAL(Format, ...) UE_LOG(LogUE4SDK, Fatal, Format, ##__VA_ARGS__)

// Assert macros
#define UE_CHECK(Expression) \
    do { if (!(Expression)) { UE_LOG_ERROR("Assertion failed: %s", #Expression); } } while(0)

#define UE_CHECKF(Expression, Format, ...) \
    do { if (!(Expression)) { UE_LOG_ERROR("Assertion failed: %s - " Format, #Expression, ##__VA_ARGS__); } } while(0)

// Force inline
#define FORCEINLINE inline

// Engine API export
#define UE4SDK_API

// Main SDK class
namespace UE4SDK
{
    /**
     * Main UE4 SDK class - Your gateway to Unreal Engine 4.26
     * Clean, simple, and powerful interface for all UE4 functionality
     */
    class UE4SDK_API SDK
    {
    public:
        // Singleton access
        static SDK& Get();
        
        // Initialization
        bool Initialize();
        void Shutdown();
        
        // Core systems
        UEngine& GetEngine();
        UWorld& GetWorld();
        Utils& GetUtils();
        
        // Version info
        static const char* GetVersion() { return UE4_SDK_VERSION; }
        
    private:
        SDK() = default;
        ~SDK() = default;
        
        SDK(const SDK&) = delete;
        SDK& operator=(const SDK&) = delete;
        
        std::unique_ptr<UEngine> m_Engine;
        std::unique_ptr<Utils> m_Utils;
    };
}
