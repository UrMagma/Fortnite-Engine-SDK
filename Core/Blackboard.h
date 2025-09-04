/*
 * UE4 Blackboard System - Self-contained Blackboard implementation
 * Vibe coder approved ✨
 */

#pragma once

#include "Types.h"
#include "UObject.h"
#include <memory>
#include <functional>

namespace UE4SDK
{
    // Forward declarations
    class BlackboardKey;
    class BlackboardData;
    class BlackboardComponent;

    // Blackboard key types
    enum class EBlackboardKeyType : uint8
    {
        Invalid = 0,
        Object = 1,
        Class = 2,
        Enum = 3,
        Int = 4,
        Float = 5,
        Bool = 6,
        String = 7,
        Vector = 8,
        Rotator = 9,
        Name = 10
    };

    // Blackboard key entry
    class BlackboardKey
    {
    public:
        BlackboardKey(const FString& InKeyName, EBlackboardKeyType InKeyType);
        virtual ~BlackboardKey() = default;

        // Key info
        const FString& GetKeyName() const { return m_KeyName; }
        EBlackboardKeyType GetKeyType() const { return m_KeyType; }
        
        // Key description
        void SetDescription(const FString& Description) { m_Description = Description; }
        const FString& GetDescription() const { return m_Description; }
        
        // Key validation
        bool IsValid() const { return !m_KeyName.IsEmpty() && m_KeyType != EBlackboardKeyType::Invalid; }
        
        // Key comparison
        bool operator==(const BlackboardKey& Other) const;
        bool operator!=(const BlackboardKey& Other) const;

    private:
        FString m_KeyName;
        EBlackboardKeyType m_KeyType;
        FString m_Description;
    };

    // Blackboard data - defines the structure of a blackboard
    class BlackboardData : public UObject
    {
    public:
        BlackboardData();
        virtual ~BlackboardData() = default;

        // Key management
        void AddKey(const FString& KeyName, EBlackboardKeyType KeyType, const FString& Description = TEXT(""));
        void RemoveKey(const FString& KeyName);
        bool HasKey(const FString& KeyName) const;
        BlackboardKey* GetKey(const FString& KeyName) const;
        TArray<BlackboardKey*> GetAllKeys() const;
        TArray<FString> GetKeyNames() const;

        // Key type queries
        EBlackboardKeyType GetKeyType(const FString& KeyName) const;
        bool IsKeyOfType(const FString& KeyName, EBlackboardKeyType KeyType) const;

        // Data info
        void SetDataName(const FString& Name) { m_DataName = Name; }
        const FString& GetDataName() const { return m_DataName; }
        
        int32 GetKeyCount() const { return static_cast<int32>(m_Keys.Num()); }

        // Data validation
        bool IsValid() const;
        TArray<FString> ValidateData() const;

        // Data serialization
        FString SerializeData() const;
        bool DeserializeData(const FString& Data);

    private:
        FString m_DataName;
        TArray<std::unique_ptr<BlackboardKey>> m_Keys;
        TMap<FString, BlackboardKey*> m_KeyMap;
    };

    // Blackboard - runtime blackboard instance
    class Blackboard : public UObject
    {
    public:
        Blackboard();
        virtual ~Blackboard() = default;

        // Data management
        void SetBlackboardData(BlackboardData* InData);
        BlackboardData* GetBlackboardData() const { return m_BlackboardData; }

        // Value management
        void SetValueAsObject(const FString& KeyName, UObject* ObjectValue);
        void SetValueAsClass(const FString& KeyName, UClass* ClassValue);
        void SetValueAsEnum(const FString& KeyName, int32 EnumValue);
        void SetValueAsInt(const FString& KeyName, int32 IntValue);
        void SetValueAsFloat(const FString& KeyName, float FloatValue);
        void SetValueAsBool(const FString& KeyName, bool BoolValue);
        void SetValueAsString(const FString& KeyName, const FString& StringValue);
        void SetValueAsVector(const FString& KeyName, const FVector& VectorValue);
        void SetValueAsRotator(const FString& KeyName, const FRotator& RotatorValue);
        void SetValueAsName(const FString& KeyName, const FName& NameValue);

        UObject* GetValueAsObject(const FString& KeyName) const;
        UClass* GetValueAsClass(const FString& KeyName) const;
        int32 GetValueAsEnum(const FString& KeyName) const;
        int32 GetValueAsInt(const FString& KeyName) const;
        float GetValueAsFloat(const FString& KeyName) const;
        bool GetValueAsBool(const FString& KeyName) const;
        FString GetValueAsString(const FString& KeyName) const;
        FVector GetValueAsVector(const FString& KeyName) const;
        FRotator GetValueAsRotator(const FString& KeyName) const;
        FName GetValueAsName(const FString& KeyName) const;

        // Value existence
        bool IsValidKey(const FString& KeyName) const;
        bool HasValue(const FString& KeyName) const;
        bool IsValueSet(const FString& KeyName) const;

        // Clear values
        void ClearValue(const FString& KeyName);
        void ClearAllValues();
        void ClearUnusedValues();

        // Blackboard info
        void SetBlackboardName(const FString& Name) { m_BlackboardName = Name; }
        const FString& GetBlackboardName() const { return m_BlackboardName; }

        // Value change notifications
        void SetOnValueChangedCallback(const FString& KeyName, std::function<void()> Callback);
        void ClearOnValueChangedCallback(const FString& KeyName);
        void SetOnAnyValueChangedCallback(std::function<void(const FString&)> Callback);

        // Blackboard state
        bool IsInitialized() const { return m_BlackboardData != nullptr; }
        void Initialize();
        void Shutdown();

        // Value iteration
        void ForEachValue(std::function<void(const FString&, EBlackboardKeyType)> Function) const;
        TArray<FString> GetSetKeys() const;
        TArray<FString> GetUnsetKeys() const;

        // Value validation
        bool ValidateValue(const FString& KeyName, EBlackboardKeyType ExpectedType) const;
        TArray<FString> ValidateAllValues() const;

        // Blackboard copying
        void CopyFrom(const Blackboard* Other);
        void CopyTo(Blackboard* Other) const;

        // Debugging
        void LogBlackboardState() const;
        FString GetBlackboardStateString() const;

    private:
        FString m_BlackboardName;
        BlackboardData* m_BlackboardData;
        
        // Value storage
        TMap<FString, UObject*> m_ObjectValues;
        TMap<FString, UClass*> m_ClassValues;
        TMap<FString, int32> m_IntValues;
        TMap<FString, float> m_FloatValues;
        TMap<FString, bool> m_BoolValues;
        TMap<FString, FString> m_StringValues;
        TMap<FString, FVector> m_VectorValues;
        TMap<FString, FRotator> m_RotatorValues;
        TMap<FString, FName> m_NameValues;
        
        // Value change callbacks
        TMap<FString, std::function<void()>> m_ValueChangedCallbacks;
        std::function<void(const FString&)> m_AnyValueChangedCallback;
        
        // Helper functions
        void NotifyValueChanged(const FString& KeyName);
        bool IsKeyRegistered(const FString& KeyName) const;
        EBlackboardKeyType GetRegisteredKeyType(const FString& KeyName) const;
    };

    // Blackboard component - component for actors to use blackboards
    class BlackboardComponent : public UObject
    {
    public:
        BlackboardComponent();
        virtual ~BlackboardComponent() = default;

        // Blackboard access
        void SetBlackboard(Blackboard* InBlackboard);
        Blackboard* GetBlackboard() const { return m_Blackboard; }

        // Component info
        void SetComponentName(const FString& Name) { m_ComponentName = Name; }
        const FString& GetComponentName() const { return m_ComponentName; }

        // Owner
        void SetOwner(UObject* Owner) { m_Owner = Owner; }
        UObject* GetOwner() const { return m_Owner; }

        // Value access (delegates to blackboard)
        void SetValueAsObject(const FString& KeyName, UObject* ObjectValue);
        void SetValueAsInt(const FString& KeyName, int32 IntValue);
        void SetValueAsFloat(const FString& KeyName, float FloatValue);
        void SetValueAsBool(const FString& KeyName, bool BoolValue);
        void SetValueAsString(const FString& KeyName, const FString& StringValue);
        void SetValueAsVector(const FString& KeyName, const FVector& VectorValue);
        void SetValueAsRotator(const FString& KeyName, const FRotator& RotatorValue);

        UObject* GetValueAsObject(const FString& KeyName) const;
        int32 GetValueAsInt(const FString& KeyName) const;
        float GetValueAsFloat(const FString& KeyName) const;
        bool GetValueAsBool(const FString& KeyName) const;
        FString GetValueAsString(const FString& KeyName) const;
        FVector GetValueAsVector(const FString& KeyName) const;
        FRotator GetValueAsRotator(const FString& KeyName) const;

        // Component state
        bool IsValid() const { return m_Blackboard != nullptr && m_Blackboard->IsInitialized(); }
        void Initialize();
        void Shutdown();

    private:
        FString m_ComponentName;
        UObject* m_Owner;
        Blackboard* m_Blackboard;
    };

    // Blackboard system - global blackboard management
    class BlackboardSystem
    {
    public:
        static BlackboardSystem& Get();

        // Blackboard data management
        BlackboardData* CreateBlackboardData(const FString& DataName);
        void DestroyBlackboardData(BlackboardData* Data);
        BlackboardData* FindBlackboardData(const FString& DataName) const;
        TArray<BlackboardData*> GetAllBlackboardData() const;

        // Blackboard management
        Blackboard* CreateBlackboard(const FString& BlackboardName, BlackboardData* Data = nullptr);
        void DestroyBlackboard(Blackboard* Blackboard);
        Blackboard* FindBlackboard(const FString& BlackboardName) const;
        TArray<Blackboard*> GetAllBlackboards() const;

        // Blackboard component management
        BlackboardComponent* CreateBlackboardComponent(const FString& ComponentName);
        void DestroyBlackboardComponent(BlackboardComponent* Component);
        BlackboardComponent* FindBlackboardComponent(const FString& ComponentName) const;
        TArray<BlackboardComponent*> GetAllBlackboardComponents() const;

        // System info
        int32 GetBlackboardDataCount() const;
        int32 GetBlackboardCount() const;
        int32 GetBlackboardComponentCount() const;

        // System state
        void Initialize();
        void Shutdown();
        void Cleanup();

    private:
        BlackboardSystem() = default;
        ~BlackboardSystem() = default;

        BlackboardSystem(const BlackboardSystem&) = delete;
        BlackboardSystem& operator=(const BlackboardSystem&) = delete;

        TArray<std::unique_ptr<BlackboardData>> m_BlackboardData;
        TArray<std::unique_ptr<Blackboard>> m_Blackboards;
        TArray<std::unique_ptr<BlackboardComponent>> m_BlackboardComponents;

        static BlackboardSystem* s_Instance;
    };

    // Global access macros
    #define BLACKBOARD_SYSTEM UE4SDK::BlackboardSystem::Get()
    #define CREATE_BLACKBOARD_DATA(Name) BLACKBOARD_SYSTEM.CreateBlackboardData(Name)
    #define CREATE_BLACKBOARD(Name, Data) BLACKBOARD_SYSTEM.CreateBlackboard(Name, Data)
    #define CREATE_BLACKBOARD_COMPONENT(Name) BLACKBOARD_SYSTEM.CreateBlackboardComponent(Name)
}
