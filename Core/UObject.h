/*
 * UE4 UObject System - Self-contained UObject and GObjects implementation
 * Vibe coder approved ✨
 */

#pragma once

#include "Types.h"
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <functional>

namespace UE4SDK
{
    // Forward declarations
    class UClass;
    class UWorld;
    class UObject;
    class GObjects;

    // Object flags
    enum class EObjectFlags : uint32
    {
        RF_NoFlags = 0x00000000,
        RF_Public = 0x00000001,
        RF_Standalone = 0x00000002,
        RF_MarkAsNative = 0x00000004,
        RF_Transactional = 0x00000008,
        RF_ClassDefaultObject = 0x00000010,
        RF_ArchetypeObject = 0x00000020,
        RF_Transient = 0x00000040,
        RF_MarkAsRootSet = 0x00000080,
        RF_TagGarbageTemp = 0x00000100,
        RF_NeedInitialization = 0x00000200,
        RF_NeedLoad = 0x00000400,
        RF_KeepForCooker = 0x00000800,
        RF_NeedPostLoad = 0x00001000,
        RF_NeedPostLoadSubobjects = 0x00002000,
        RF_NewerVersionExists = 0x00004000,
        RF_BeginDestroyed = 0x00008000,
        RF_FinishDestroyed = 0x00010000,
        RF_BeingRegenerated = 0x00020000,
        RF_DefaultSubObject = 0x00040000,
        RF_WasLoaded = 0x00080000,
        RF_TextExportTransient = 0x00100000,
        RF_LoadCompleted = 0x00200000,
        RF_InheritableComponentTemplate = 0x00400000,
        RF_DuplicateTransient = 0x00800000,
        RF_StrongRefOnFrame = 0x01000000,
        RF_NonPIEDuplicateTransient = 0x02000000,
        RF_Dynamic = 0x04000000,
        RF_WillBeLoaded = 0x08000000
    };

    // Object state
    enum class EObjectState : uint8
    {
        None = 0,
        Created = 1,
        Initialized = 2,
        Active = 3,
        PendingKill = 4,
        Destroyed = 5
    };

    // UClass - Class metadata and reflection
    class UClass
    {
    public:
        UClass(const FString& InClassName, UClass* InSuperClass = nullptr);
        virtual ~UClass() = default;

        // Class info
        const FString& GetClassName() const { return m_ClassName; }
        UClass* GetSuperClass() const { return m_SuperClass; }
        bool IsChildOf(UClass* InClass) const;
        bool IsA(UClass* InClass) const;

        // Object creation
        virtual UObject* CreateObject(UObject* Outer = nullptr, const FString& Name = TEXT(""));

        // Properties
        void AddProperty(const FString& PropertyName, const FString& PropertyType);
        bool HasProperty(const FString& PropertyName) const;
        TArray<FString> GetPropertyNames() const;

        // Functions
        void AddFunction(const FString& FunctionName, std::function<void(UObject*, const TArray<FString>&)> Function);
        bool HasFunction(const FString& FunctionName) const;
        void CallFunction(UObject* Object, const FString& FunctionName, const TArray<FString>& Parameters);

    private:
        FString m_ClassName;
        UClass* m_SuperClass;
        TArray<FString> m_PropertyNames;
        TMap<FString, std::function<void(UObject*, const TArray<FString>&)>> m_Functions;
    };

    // UObject - Base object class
    class UObject
    {
    public:
        UObject();
        virtual ~UObject();

        // Object info
        const FString& GetName() const { return m_Name; }
        void SetName(const FString& NewName) { m_Name = NewName; }
        
        UClass* GetClass() const { return m_Class; }
        void SetClass(UClass* NewClass) { m_Class = NewClass; }
        
        UObject* GetOuter() const { return m_Outer; }
        void SetOuter(UObject* NewOuter) { m_Outer = NewOuter; }

        // Object state
        EObjectState GetState() const { return m_State; }
        void SetState(EObjectState NewState) { m_State = NewState; }
        
        bool IsValid() const { return m_State != EObjectState::Destroyed && m_State != EObjectState::PendingKill; }
        bool IsPendingKill() const { return m_State == EObjectState::PendingKill; }

        // Object flags
        EObjectFlags GetFlags() const { return m_Flags; }
        void SetFlags(EObjectFlags NewFlags) { m_Flags = NewFlags; }
        bool HasFlag(EObjectFlags Flag) const { return (m_Flags & Flag) != EObjectFlags::RF_NoFlags; }
        void AddFlag(EObjectFlags Flag) { m_Flags = static_cast<EObjectFlags>(static_cast<uint32>(m_Flags) | static_cast<uint32>(Flag)); }
        void RemoveFlag(EObjectFlags Flag) { m_Flags = static_cast<EObjectFlags>(static_cast<uint32>(m_Flags) & ~static_cast<uint32>(Flag)); }

        // Lifecycle
        virtual void BeginDestroy();
        virtual void FinishDestroy();
        virtual void MarkPendingKill();
        virtual void ClearPendingKill();

        // Properties
        virtual void SetProperty(const FString& PropertyName, const FString& Value);
        virtual FString GetProperty(const FString& PropertyName) const;
        virtual bool HasProperty(const FString& PropertyName) const;

        // Functions
        virtual void CallFunction(const FString& FunctionName, const TArray<FString>& Parameters = TArray<FString>());
        virtual bool HasFunction(const FString& FunctionName) const;

        // Tags
        void AddTag(const FString& Tag);
        void RemoveTag(const FString& Tag);
        bool HasTag(const FString& Tag) const;
        TArray<FString> GetAllTags() const;

        // Custom data
        void SetCustomData(const FString& Key, const FString& Value);
        FString GetCustomData(const FString& Key) const;
        bool HasCustomData(const FString& Key) const;
        void RemoveCustomData(const FString& Key);

        // Object ID
        uint32 GetObjectID() const { return m_ObjectID; }

    protected:
        FString m_Name;
        UClass* m_Class;
        UObject* m_Outer;
        EObjectState m_State;
        EObjectFlags m_Flags;
        uint32 m_ObjectID;
        TArray<FString> m_Tags;
        TMap<FString, FString> m_CustomData;

        static uint32 s_NextObjectID;
    };

    // GObjects - Global object manager
    class GObjects
    {
    public:
        static GObjects& Get();
        
        // Object management
        UObject* CreateObject(const FString& ClassName, UObject* Outer = nullptr, const FString& Name = TEXT(""));
        void DestroyObject(UObject* Object);
        void DestroyObject(uint32 ObjectID);
        
        // Object queries
        UObject* FindObject(const FString& Name) const;
        UObject* FindObject(uint32 ObjectID) const;
        TArray<UObject*> FindObjectsOfClass(UClass* Class) const;
        TArray<UObject*> FindObjectsOfClass(const FString& ClassName) const;
        TArray<UObject*> GetAllObjects() const;
        
        // Object registration
        void RegisterObject(UObject* Object);
        void UnregisterObject(UObject* Object);
        
        // Garbage collection
        void MarkForGarbageCollection(UObject* Object);
        void CollectGarbage();
        void ForceGarbageCollection();
        
        // Object counting
        int32 GetObjectCount() const { return static_cast<int32>(m_Objects.size()); }
        int32 GetObjectCountOfClass(UClass* Class) const;
        int32 GetObjectCountOfClass(const FString& ClassName) const;
        
        // Class management
        UClass* FindClass(const FString& ClassName) const;
        UClass* RegisterClass(const FString& ClassName, UClass* SuperClass = nullptr);
        TArray<UClass*> GetAllClasses() const;
        
        // Object iteration
        void ForEachObject(std::function<void(UObject*)> Function);
        void ForEachObjectOfClass(UClass* Class, std::function<void(UObject*)> Function);
        
        // Cleanup
        void Cleanup();

    private:
        GObjects() = default;
        ~GObjects() = default;
        
        GObjects(const GObjects&) = delete;
        GObjects& operator=(const GObjects&) = delete;
        
        std::unordered_map<uint32, std::unique_ptr<UObject>> m_Objects;
        std::unordered_map<FString, std::unique_ptr<UClass>> m_Classes;
        TSet<UObject*> m_PendingKillObjects;
        
        static GObjects* s_Instance;
    };

    // Global access macros
    #define GOBJECTS UE4SDK::GObjects::Get()
    #define NEW_OBJECT(ClassName, Outer, Name) GOBJECTS.CreateObject(TEXT(ClassName), Outer, Name)
    #define DESTROY_OBJECT(Object) GOBJECTS.DestroyObject(Object)
    #define FIND_OBJECT(Name) GOBJECTS.FindObject(Name)
    #define FIND_OBJECTS_OF_CLASS(ClassName) GOBJECTS.FindObjectsOfClass(ClassName)
}
