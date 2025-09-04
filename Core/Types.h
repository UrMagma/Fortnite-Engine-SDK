/*
 * UE4 Core Types - Embedded UE4 type definitions
 * Self-contained, vibe coder approved ✨
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
#include <chrono>
#include <random>

namespace UE4SDK
{
    // Basic types
    using int8 = int8_t;
    using int16 = int16_t;
    using int32 = int32_t;
    using int64 = int64_t;
    using uint8 = uint8_t;
    using uint16 = uint16_t;
    using uint32 = uint32_t;
    using uint64 = uint64_t;
    using float32 = float;
    using float64 = double;
    using bool8 = bool;

    // String type
    class FString
    {
    public:
        FString() = default;
        FString(const char* InString) : m_String(InString ? InString : "") {}
        FString(const std::string& InString) : m_String(InString) {}
        FString(const FString& Other) : m_String(Other.m_String) {}
        
        FString& operator=(const FString& Other) { m_String = Other.m_String; return *this; }
        FString& operator=(const char* InString) { m_String = InString ? InString : ""; return *this; }
        
        bool operator==(const FString& Other) const { return m_String == Other.m_String; }
        bool operator!=(const FString& Other) const { return m_String != Other.m_String; }
        bool operator<(const FString& Other) const { return m_String < Other.m_String; }
        
        FString operator+(const FString& Other) const { return FString(m_String + Other.m_String); }
        FString& operator+=(const FString& Other) { m_String += Other.m_String; return *this; }
        
        const char* operator*() const { return m_String.c_str(); }
        const char* c_str() const { return m_String.c_str(); }
        
        int32 Len() const { return static_cast<int32>(m_String.length()); }
        bool IsEmpty() const { return m_String.empty(); }
        
        FString ToUpper() const;
        FString ToLower() const;
        FString Trim() const;
        
        bool Contains(const FString& Substring) const;
        bool StartsWith(const FString& Prefix) const;
        bool EndsWith(const FString& Suffix) const;
        
        int32 Find(const FString& Substring) const;
        int32 FindLast(const FString& Substring) const;
        
        FString Left(int32 Count) const;
        FString Right(int32 Count) const;
        FString Mid(int32 Start, int32 Count = -1) const;
        
        std::vector<FString> Split(const FString& Delimiter) const;
        FString Replace(const FString& From, const FString& To) const;
        
        int32 ToInt() const;
        float ToFloat() const;
        bool ToBool() const;
        
        static FString FromInt(int32 Value);
        static FString FromFloat(float Value, int32 DecimalPlaces = 2);
        static FString FromBool(bool Value);
        
    private:
        std::string m_String;
    };

    // Name type
    class FName
    {
    public:
        FName() = default;
        FName(const char* InName) : m_Name(InName ? InName : "") {}
        FName(const FString& InName) : m_Name(InName.c_str()) {}
        
        bool operator==(const FName& Other) const { return m_Name == Other.m_Name; }
        bool operator!=(const FName& Other) const { return m_Name != Other.m_Name; }
        bool operator<(const FName& Other) const { return m_Name < Other.m_Name; }
        
        const char* operator*() const { return m_Name.c_str(); }
        const char* c_str() const { return m_Name.c_str(); }
        
        FString ToString() const { return FString(m_Name.c_str()); }
        
    private:
        std::string m_Name;
    };

    // Vector type
    class FVector
    {
    public:
        float X, Y, Z;
        
        FVector() : X(0.0f), Y(0.0f), Z(0.0f) {}
        FVector(float InX, float InY, float InZ) : X(InX), Y(InY), Z(InZ) {}
        FVector(float InValue) : X(InValue), Y(InValue), Z(InValue) {}
        
        bool operator==(const FVector& Other) const;
        bool operator!=(const FVector& Other) const;
        
        FVector operator+(const FVector& Other) const;
        FVector operator-(const FVector& Other) const;
        FVector operator*(const FVector& Other) const;
        FVector operator*(float Scale) const;
        FVector operator/(const FVector& Other) const;
        FVector operator/(float Scale) const;
        FVector operator-() const;
        
        FVector& operator+=(const FVector& Other);
        FVector& operator-=(const FVector& Other);
        FVector& operator*=(const FVector& Other);
        FVector& operator*=(float Scale);
        FVector& operator/=(const FVector& Other);
        FVector& operator/=(float Scale);
        
        float operator|(const FVector& Other) const; // Dot product
        FVector operator^(const FVector& Other) const; // Cross product
        
        float Size() const;
        float SizeSquared() const;
        float Size2D() const;
        float SizeSquared2D() const;
        
        FVector GetSafeNormal(float Tolerance = 1e-8f) const;
        FVector GetUnsafeNormal() const;
        
        bool IsNearlyZero(float Tolerance = 1e-8f) const;
        bool IsZero() const;
        bool IsUnit(float LengthSquaredTolerance = 1e-8f) const;
        
        bool ContainsNaN() const;
        bool IsFinite() const;
        
        FVector RotateAngleAxis(float AngleDeg, const FVector& Axis) const;
        
        FString ToString() const;
        
        // Static constants
        static const FVector ZeroVector;
        static const FVector OneVector;
        static const FVector UpVector;
        static const FVector DownVector;
        static const FVector ForwardVector;
        static const FVector BackwardVector;
        static const FVector RightVector;
        static const FVector LeftVector;
    };

    // Rotator type
    class FRotator
    {
    public:
        float Pitch, Yaw, Roll;
        
        FRotator() : Pitch(0.0f), Yaw(0.0f), Roll(0.0f) {}
        FRotator(float InPitch, float InYaw, float InRoll) : Pitch(InPitch), Yaw(InYaw), Roll(InRoll) {}
        FRotator(float InValue) : Pitch(InValue), Yaw(InValue), Roll(InValue) {}
        
        bool operator==(const FRotator& Other) const;
        bool operator!=(const FRotator& Other) const;
        
        FRotator operator+(const FRotator& Other) const;
        FRotator operator-(const FRotator& Other) const;
        FRotator operator*(const FRotator& Other) const;
        FRotator operator*(float Scale) const;
        FRotator operator-() const;
        
        FRotator& operator+=(const FRotator& Other);
        FRotator& operator-=(const FRotator& Other);
        FRotator& operator*=(const FRotator& Other);
        FRotator& operator*=(float Scale);
        
        bool IsNearlyZero(float Tolerance = 1e-8f) const;
        bool IsZero() const;
        
        bool ContainsNaN() const;
        bool IsFinite() const;
        
        FRotator GetNormalized() const;
        FRotator GetDenormalized() const;
        
        FVector Vector() const;
        FVector GetForwardVector() const;
        FVector GetRightVector() const;
        FVector GetUpVector() const;
        
        FString ToString() const;
        
        // Static constants
        static const FRotator ZeroRotator;
    };

    // Quaternion type
    class FQuat
    {
    public:
        float X, Y, Z, W;
        
        FQuat() : X(0.0f), Y(0.0f), Z(0.0f), W(1.0f) {}
        FQuat(float InX, float InY, float InZ, float InW) : X(InX), Y(InY), Z(InZ), W(InW) {}
        
        bool operator==(const FQuat& Other) const;
        bool operator!=(const FQuat& Other) const;
        
        FQuat operator+(const FQuat& Other) const;
        FQuat operator-(const FQuat& Other) const;
        FQuat operator*(const FQuat& Other) const;
        FQuat operator*(float Scale) const;
        FQuat operator-() const;
        
        FQuat& operator+=(const FQuat& Other);
        FQuat& operator-=(const FQuat& Other);
        FQuat& operator*=(const FQuat& Other);
        FQuat& operator*=(float Scale);
        
        float Size() const;
        float SizeSquared() const;
        
        FQuat GetNormalized() const;
        bool IsNormalized() const;
        
        bool IsNearlyZero(float Tolerance = 1e-8f) const;
        bool IsZero() const;
        
        bool ContainsNaN() const;
        bool IsFinite() const;
        
        FVector RotateVector(const FVector& V) const;
        FVector GetAxisX() const;
        FVector GetAxisY() const;
        FVector GetAxisZ() const;
        
        FString ToString() const;
        
        // Static constants
        static const FQuat Identity;
    };

    // Transform type
    class FTransform
    {
    public:
        FQuat Rotation;
        FVector Translation;
        FVector Scale3D;
        
        FTransform() : Rotation(FQuat::Identity), Translation(FVector::ZeroVector), Scale3D(FVector::OneVector) {}
        FTransform(const FQuat& InRotation, const FVector& InTranslation, const FVector& InScale3D = FVector::OneVector)
            : Rotation(InRotation), Translation(InTranslation), Scale3D(InScale3D) {}
        FTransform(const FRotator& InRotation, const FVector& InTranslation, const FVector& InScale3D = FVector::OneVector);
        
        bool operator==(const FTransform& Other) const;
        bool operator!=(const FTransform& Other) const;
        
        FTransform operator*(const FTransform& Other) const;
        FTransform& operator*=(const FTransform& Other);
        
        FVector TransformPosition(const FVector& V) const;
        FVector TransformVector(const FVector& V) const;
        FVector TransformVectorNoScale(const FVector& V) const;
        FVector InverseTransformPosition(const FVector& V) const;
        FVector InverseTransformVector(const FVector& V) const;
        FVector InverseTransformVectorNoScale(const FVector& V) const;
        
        FTransform GetRelativeTransform(const FTransform& Other) const;
        FTransform GetRelativeTransformReverse(const FTransform& Other) const;
        
        FTransform GetScaled(float InScale) const;
        FTransform GetScaled(const FVector& InScale) const;
        FTransform GetNormalized() const;
        
        bool IsRotationNormalized() const;
        bool IsTranslationValid() const;
        bool IsScale3DValid() const;
        
        bool ContainsNaN() const;
        bool IsFinite() const;
        
        FString ToString() const;
        
        // Static constants
        static const FTransform Identity;
    };

    // Color types
    class FLinearColor
    {
    public:
        float R, G, B, A;
        
        FLinearColor() : R(0.0f), G(0.0f), B(0.0f), A(1.0f) {}
        FLinearColor(float InR, float InG, float InB, float InA = 1.0f) : R(InR), G(InG), B(InB), A(InA) {}
        FLinearColor(const FVector& InVector, float InA = 1.0f) : R(InVector.X), G(InVector.Y), B(InVector.Z), A(InA) {}
        
        bool operator==(const FLinearColor& Other) const;
        bool operator!=(const FLinearColor& Other) const;
        
        FLinearColor operator+(const FLinearColor& Other) const;
        FLinearColor operator-(const FLinearColor& Other) const;
        FLinearColor operator*(const FLinearColor& Other) const;
        FLinearColor operator*(float Scale) const;
        FLinearColor operator/(const FLinearColor& Other) const;
        FLinearColor operator/(float Scale) const;
        
        FLinearColor& operator+=(const FLinearColor& Other);
        FLinearColor& operator-=(const FLinearColor& Other);
        FLinearColor& operator*=(const FLinearColor& Other);
        FLinearColor& operator*=(float Scale);
        FLinearColor& operator/=(const FLinearColor& Other);
        FLinearColor& operator/=(float Scale);
        
        FLinearColor GetClamped() const;
        FLinearColor GetClamped(float Min, float Max) const;
        
        bool IsAlmostBlack() const;
        bool IsAlmostWhite() const;
        
        FString ToString() const;
        
        // Static constants
        static const FLinearColor White;
        static const FLinearColor Black;
        static const FLinearColor Red;
        static const FLinearColor Green;
        static const FLinearColor Blue;
        static const FLinearColor Yellow;
        static const FLinearColor Cyan;
        static const FLinearColor Magenta;
        static const FLinearColor Transparent;
    };

    class FColor
    {
    public:
        uint8 R, G, B, A;
        
        FColor() : R(0), G(0), B(0), A(255) {}
        FColor(uint8 InR, uint8 InG, uint8 InB, uint8 InA = 255) : R(InR), G(InG), B(InB), A(InA) {}
        FColor(const FLinearColor& InLinearColor);
        
        bool operator==(const FColor& Other) const;
        bool operator!=(const FColor& Other) const;
        
        FLinearColor ReinterpretAsLinear() const;
        FString ToString() const;
        
        // Static constants
        static const FColor White;
        static const FColor Black;
        static const FColor Red;
        static const FColor Green;
        static const FColor Blue;
        static const FColor Yellow;
        static const FColor Cyan;
        static const FColor Magenta;
        static const FColor Transparent;
    };

    // Container types
    template<typename T>
    class TArray
    {
    public:
        using ElementType = T;
        using SizeType = int32;
        using IndexType = int32;
        
        TArray() = default;
        TArray(const TArray& Other) : m_Data(Other.m_Data) {}
        TArray(TArray&& Other) noexcept : m_Data(std::move(Other.m_Data)) {}
        
        TArray& operator=(const TArray& Other) { m_Data = Other.m_Data; return *this; }
        TArray& operator=(TArray&& Other) noexcept { m_Data = std::move(Other.m_Data); return *this; }
        
        T& operator[](IndexType Index) { return m_Data[Index]; }
        const T& operator[](IndexType Index) const { return m_Data[Index]; }
        
        T& operator[](SizeType Index) { return m_Data[Index]; }
        const T& operator[](SizeType Index) const { return m_Data[Index]; }
        
        SizeType Num() const { return static_cast<SizeType>(m_Data.size()); }
        SizeType Max() const { return static_cast<SizeType>(m_Data.capacity()); }
        bool IsEmpty() const { return m_Data.empty(); }
        
        T& Add(const T& Item) { m_Data.push_back(Item); return m_Data.back(); }
        T& Add(T&& Item) { m_Data.push_back(std::move(Item)); return m_Data.back(); }
        T& Emplace() { m_Data.emplace_back(); return m_Data.back(); }
        
        template<typename... Args>
        T& Emplace(Args&&... Args) { m_Data.emplace_back(std::forward<Args>(Args)...); return m_Data.back(); }
        
        void RemoveAt(IndexType Index);
        void RemoveAtSwap(IndexType Index);
        bool Remove(const T& Item);
        bool RemoveSwap(const T& Item);
        
        void Empty() { m_Data.clear(); }
        void Reset() { m_Data.clear(); m_Data.shrink_to_fit(); }
        
        void Reserve(SizeType NewCapacity) { m_Data.reserve(NewCapacity); }
        void SetNum(SizeType NewNum);
        void SetNum(SizeType NewNum, const T& Value);
        
        T& Last() { return m_Data.back(); }
        const T& Last() const { return m_Data.back(); }
        
        T& Top() { return m_Data.back(); }
        const T& Top() const { return m_Data.back(); }
        
        void Pop() { m_Data.pop_back(); }
        T PopValue() { T Value = std::move(m_Data.back()); m_Data.pop_back(); return Value; }
        
        IndexType Find(const T& Item) const;
        bool Contains(const T& Item) const;
        
        void Sort();
        template<typename Predicate>
        void Sort(Predicate Pred);
        
        void Reverse();
        
        T* GetData() { return m_Data.data(); }
        const T* GetData() const { return m_Data.data(); }
        
        auto begin() { return m_Data.begin(); }
        auto end() { return m_Data.end(); }
        auto begin() const { return m_Data.begin(); }
        auto end() const { return m_Data.end(); }
        
    private:
        std::vector<T> m_Data;
    };

    template<typename K, typename V>
    class TMap
    {
    public:
        using KeyType = K;
        using ValueType = V;
        using SizeType = int32;
        
        TMap() = default;
        TMap(const TMap& Other) : m_Data(Other.m_Data) {}
        TMap(TMap&& Other) noexcept : m_Data(std::move(Other.m_Data)) {}
        
        TMap& operator=(const TMap& Other) { m_Data = Other.m_Data; return *this; }
        TMap& operator=(TMap&& Other) noexcept { m_Data = std::move(Other.m_Data); return *this; }
        
        V& operator[](const K& Key) { return m_Data[Key]; }
        const V& operator[](const K& Key) const { return m_Data.at(Key); }
        
        SizeType Num() const { return static_cast<SizeType>(m_Data.size()); }
        bool IsEmpty() const { return m_Data.empty(); }
        
        V& Add(const K& Key, const V& Value) { return m_Data[Key] = Value; }
        V& Add(const K& Key, V&& Value) { return m_Data[Key] = std::move(Value); }
        
        bool Remove(const K& Key) { return m_Data.erase(Key) > 0; }
        void Empty() { m_Data.clear(); }
        void Reset() { m_Data.clear(); }
        
        bool Contains(const K& Key) const { return m_Data.find(Key) != m_Data.end(); }
        V* Find(const K& Key) { auto It = m_Data.find(Key); return It != m_Data.end() ? &It->second : nullptr; }
        const V* Find(const K& Key) const { auto It = m_Data.find(Key); return It != m_Data.end() ? &It->second : nullptr; }
        
        V& FindOrAdd(const K& Key) { return m_Data[Key]; }
        V& FindOrAdd(const K& Key, const V& DefaultValue) { return m_Data.emplace(Key, DefaultValue).first->second; }
        
        TArray<K> GetKeys() const;
        TArray<V> GetValues() const;
        
        auto begin() { return m_Data.begin(); }
        auto end() { return m_Data.end(); }
        auto begin() const { return m_Data.begin(); }
        auto end() const { return m_Data.end(); }
        
    private:
        std::map<K, V> m_Data;
    };

    template<typename T>
    class TSet
    {
    public:
        using ElementType = T;
        using SizeType = int32;
        
        TSet() = default;
        TSet(const TSet& Other) : m_Data(Other.m_Data) {}
        TSet(TSet&& Other) noexcept : m_Data(std::move(Other.m_Data)) {}
        
        TSet& operator=(const TSet& Other) { m_Data = Other.m_Data; return *this; }
        TSet& operator=(TSet&& Other) noexcept { m_Data = std::move(Other.m_Data); return *this; }
        
        SizeType Num() const { return static_cast<SizeType>(m_Data.size()); }
        bool IsEmpty() const { return m_Data.empty(); }
        
        void Add(const T& Item) { m_Data.insert(Item); }
        void Add(T&& Item) { m_Data.insert(std::move(Item)); }
        
        bool Remove(const T& Item) { return m_Data.erase(Item) > 0; }
        void Empty() { m_Data.clear(); }
        void Reset() { m_Data.clear(); }
        
        bool Contains(const T& Item) const { return m_Data.find(Item) != m_Data.end(); }
        
        TArray<T> Array() const;
        
        auto begin() { return m_Data.begin(); }
        auto end() { return m_Data.end(); }
        auto begin() const { return m_Data.begin(); }
        auto end() const { return m_Data.end(); }
        
    private:
        std::set<T> m_Data;
    };

    // Enums
    enum class ELogVerbosity : uint8
    {
        NoLogging = 0,
        Fatal = 1,
        Error = 2,
        Warning = 3,
        Display = 4,
        Log = 5,
        Verbose = 6,
        VeryVerbose = 7
    };

    enum class ECollisionChannel : uint8
    {
        ECC_WorldStatic = 0,
        ECC_WorldDynamic = 1,
        ECC_Pawn = 2,
        ECC_Visibility = 3,
        ECC_Camera = 4,
        ECC_PhysicsBody = 5,
        ECC_Vehicle = 6,
        ECC_Destructible = 7,
        ECC_EngineTraceChannel1 = 8,
        ECC_EngineTraceChannel2 = 9,
        ECC_EngineTraceChannel3 = 10,
        ECC_EngineTraceChannel4 = 11,
        ECC_EngineTraceChannel5 = 12,
        ECC_EngineTraceChannel6 = 13,
        ECC_GameTraceChannel1 = 14,
        ECC_GameTraceChannel2 = 15,
        ECC_GameTraceChannel3 = 16,
        ECC_GameTraceChannel4 = 17,
        ECC_GameTraceChannel5 = 18,
        ECC_GameTraceChannel6 = 19,
        ECC_GameTraceChannel7 = 20,
        ECC_GameTraceChannel8 = 21,
        ECC_GameTraceChannel9 = 22,
        ECC_GameTraceChannel10 = 23,
        ECC_GameTraceChannel11 = 24,
        ECC_GameTraceChannel12 = 25,
        ECC_GameTraceChannel13 = 26,
        ECC_GameTraceChannel14 = 27,
        ECC_GameTraceChannel15 = 28,
        ECC_GameTraceChannel16 = 29,
        ECC_GameTraceChannel17 = 30,
        ECC_GameTraceChannel18 = 31,
        ECC_MAX = 32
    };

    enum class ECollisionResponse : uint8
    {
        ECR_Ignore = 0,
        ECR_Overlap = 1,
        ECR_Block = 2
    };

    enum class EViewModeIndex : uint8
    {
        VMI_BrushWireframe = 0,
        VMI_Wireframe = 1,
        VMI_Unlit = 2,
        VMI_Lit = 3,
        VMI_Lit_DetailLighting = 4,
        VMI_LightingOnly = 5,
        VMI_LightComplexity = 6,
        VMI_ShaderComplexity = 7,
        VMI_LightmapDensity = 8,
        VMI_LitLightmapDensity = 9,
        VMI_ReflectionOverride = 10,
        VMI_VisualizeBuffer = 11,
        VMI_StationaryLightOverlap = 12,
        VMI_CollisionPawn = 13,
        VMI_CollisionVisibility = 14,
        VMI_LODColoration = 15,
        VMI_QuadOverdraw = 16,
        VMI_PrimitiveDistanceAccuracy = 17,
        VMI_MeshUVDensityAccuracy = 18,
        VMI_ShaderComplexityWithQuadOverdraw = 19,
        VMI_HLODColoration = 20,
        VMI_GroupLODColoration = 21,
        VMI_MaterialTextureScaleAccuracy = 22,
        VMI_RequiredTextureResolution = 23,
        VMI_PathTracing = 24,
        VMI_RayTracingDebug = 25,
        VMI_Max = 26,
        VMI_Unknown = 255
    };

    enum class EInputEvent : uint8
    {
        IE_Pressed = 0,
        IE_Released = 1,
        IE_Repeat = 2,
        IE_DoubleClick = 3,
        IE_Axis = 4,
        IE_MAX = 5
    };

    enum class ENetRole : uint8
    {
        ROLE_None = 0,
        ROLE_SimulatedProxy = 1,
        ROLE_AutonomousProxy = 2,
        ROLE_Authority = 3,
        ROLE_MAX = 4
    };

    enum class EComponentMobility : uint8
    {
        Static = 0,
        Stationary = 1,
        Movable = 2
    };

    enum class ECameraProjectionMode : uint8
    {
        Perspective = 0,
        Orthographic = 1
    };

    enum class EControllerHand : uint8
    {
        Left = 0,
        Right = 1,
        AnyHand = 2,
        Pad = 3,
        ExternalCamera = 4,
        Gun = 5,
        Special_1 = 6,
        Special_2 = 7,
        Special_3 = 8,
        Special_4 = 9,
        Special_5 = 10,
        Special_6 = 11,
        Special_7 = 12,
        Special_8 = 13,
        Special_9 = 14,
        Special_10 = 15,
        Special_11 = 16,
        ControllerHand_Count = 17
    };

    enum class EEndPlayReason : uint8
    {
        Destroyed = 0,
        LevelTransition = 1,
        EndPlayInEditor = 2,
        RemovedFromWorld = 3,
        Quit = 4
    };

    enum class EAutoReceiveInput : uint8
    {
        Disabled = 0,
        Player0 = 1,
        Player1 = 2,
        Player2 = 3,
        Player3 = 4,
        Player4 = 5,
        Player5 = 6,
        Player6 = 7,
        Player7 = 8
    };

    enum class EAutoPossessAI : uint8
    {
        Disabled = 0,
        PlacedInWorld = 1,
        Spawned = 2
    };

    enum class ETravelType : uint8
    {
        TRAVEL_Absolute = 0,
        TRAVEL_Partial = 1,
        TRAVEL_Relative = 2
    };

    enum class EMouseLockMode : uint8
    {
        DoNotLock = 0,
        LockOnCapture = 1,
        LockAlways = 2,
        LockInFullscreen = 3
    };

    enum class EViewTargetBlendFunction : uint8
    {
        VTBlend_Linear = 0,
        VTBlend_Cubic = 1,
        VTBlend_EaseIn = 2,
        VTBlend_EaseOut = 3,
        VTBlend_EaseInOut = 4,
        VTBlend_MAX = 5
    };

    enum class EObjectTypeQuery : uint8
    {
        ObjectTypeQuery1 = 0,
        ObjectTypeQuery2 = 1,
        ObjectTypeQuery3 = 2,
        ObjectTypeQuery4 = 3,
        ObjectTypeQuery5 = 4,
        ObjectTypeQuery6 = 5,
        ObjectTypeQuery7 = 6,
        ObjectTypeQuery8 = 7,
        ObjectTypeQuery9 = 8,
        ObjectTypeQuery10 = 9,
        ObjectTypeQuery11 = 10,
        ObjectTypeQuery12 = 11,
        ObjectTypeQuery13 = 12,
        ObjectTypeQuery14 = 13,
        ObjectTypeQuery15 = 14,
        ObjectTypeQuery16 = 15,
        ObjectTypeQuery17 = 16,
        ObjectTypeQuery18 = 17,
        ObjectTypeQuery19 = 18,
        ObjectTypeQuery20 = 19,
        ObjectTypeQuery21 = 20,
        ObjectTypeQuery22 = 21,
        ObjectTypeQuery23 = 22,
        ObjectTypeQuery24 = 23,
        ObjectTypeQuery25 = 24,
        ObjectTypeQuery26 = 25,
        ObjectTypeQuery27 = 26,
        ObjectTypeQuery28 = 27,
        ObjectTypeQuery29 = 28,
        ObjectTypeQuery30 = 29,
        ObjectTypeQuery31 = 30,
        ObjectTypeQuery32 = 31,
        ObjectTypeQuery_MAX = 32,
        ObjectTypeQuery_AllObjects = 32
    };
}
