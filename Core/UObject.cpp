/*
 * UE4 UObject Implementation - Self-contained UObject and GObjects implementation
 * Vibe coder approved ✨
 */

#include "UObject.h"
#include <algorithm>
#include <cassert>

namespace UE4SDK
{
    // UObject static member
    uint32 UObject::s_NextObjectID = 1;

    // UClass implementation
    UClass::UClass(const FString& InClassName, UClass* InSuperClass)
        : m_ClassName(InClassName), m_SuperClass(InSuperClass)
    {
    }

    bool UClass::IsChildOf(UClass* InClass) const
    {
        if (!InClass) return false;
        if (this == InClass) return true;
        
        UClass* Current = m_SuperClass;
        while (Current)
        {
            if (Current == InClass) return true;
            Current = Current->m_SuperClass;
        }
        return false;
    }

    bool UClass::IsA(UClass* InClass) const
    {
        return IsChildOf(InClass);
    }

    UObject* UClass::CreateObject(UObject* Outer, const FString& Name)
    {
        UObject* NewObject = new UObject();
        NewObject->SetClass(this);
        NewObject->SetOuter(Outer);
        if (!Name.IsEmpty())
        {
            NewObject->SetName(Name);
        }
        return NewObject;
    }

    void UClass::AddProperty(const FString& PropertyName, const FString& PropertyType)
    {
        m_PropertyNames.Add(PropertyName);
    }

    bool UClass::HasProperty(const FString& PropertyName) const
    {
        return m_PropertyNames.Contains(PropertyName);
    }

    TArray<FString> UClass::GetPropertyNames() const
    {
        return m_PropertyNames;
    }

    void UClass::AddFunction(const FString& FunctionName, std::function<void(UObject*, const TArray<FString>&)> Function)
    {
        m_Functions.Add(FunctionName, Function);
    }

    bool UClass::HasFunction(const FString& FunctionName) const
    {
        return m_Functions.Contains(FunctionName);
    }

    void UClass::CallFunction(UObject* Object, const FString& FunctionName, const TArray<FString>& Parameters)
    {
        auto* Function = m_Functions.Find(FunctionName);
        if (Function)
        {
            (*Function)(Object, Parameters);
        }
    }

    // UObject implementation
    UObject::UObject()
        : m_Name(TEXT("")), m_Class(nullptr), m_Outer(nullptr), m_State(EObjectState::Created), m_Flags(EObjectFlags::RF_NoFlags), m_ObjectID(s_NextObjectID++)
    {
    }

    UObject::~UObject()
    {
        BeginDestroy();
    }

    void UObject::BeginDestroy()
    {
        if (m_State != EObjectState::Destroyed)
        {
            m_State = EObjectState::PendingKill;
        }
    }

    void UObject::FinishDestroy()
    {
        m_State = EObjectState::Destroyed;
    }

    void UObject::MarkPendingKill()
    {
        m_State = EObjectState::PendingKill;
    }

    void UObject::ClearPendingKill()
    {
        if (m_State == EObjectState::PendingKill)
        {
            m_State = EObjectState::Active;
        }
    }

    void UObject::SetProperty(const FString& PropertyName, const FString& Value)
    {
        // Default implementation - subclasses can override
        m_CustomData.Add(PropertyName, Value);
    }

    FString UObject::GetProperty(const FString& PropertyName) const
    {
        // Default implementation - subclasses can override
        const FString* Value = m_CustomData.Find(PropertyName);
        return Value ? *Value : FString();
    }

    bool UObject::HasProperty(const FString& PropertyName) const
    {
        return m_CustomData.Contains(PropertyName);
    }

    void UObject::CallFunction(const FString& FunctionName, const TArray<FString>& Parameters)
    {
        if (m_Class)
        {
            m_Class->CallFunction(this, FunctionName, Parameters);
        }
    }

    bool UObject::HasFunction(const FString& FunctionName) const
    {
        return m_Class ? m_Class->HasFunction(FunctionName) : false;
    }

    void UObject::AddTag(const FString& Tag)
    {
        if (!m_Tags.Contains(Tag))
        {
            m_Tags.Add(Tag);
        }
    }

    void UObject::RemoveTag(const FString& Tag)
    {
        m_Tags.Remove(Tag);
    }

    bool UObject::HasTag(const FString& Tag) const
    {
        return m_Tags.Contains(Tag);
    }

    TArray<FString> UObject::GetAllTags() const
    {
        return m_Tags;
    }

    void UObject::SetCustomData(const FString& Key, const FString& Value)
    {
        m_CustomData.Add(Key, Value);
    }

    FString UObject::GetCustomData(const FString& Key) const
    {
        const FString* Value = m_CustomData.Find(Key);
        return Value ? *Value : FString();
    }

    bool UObject::HasCustomData(const FString& Key) const
    {
        return m_CustomData.Contains(Key);
    }

    void UObject::RemoveCustomData(const FString& Key)
    {
        m_CustomData.Remove(Key);
    }

    // GObjects implementation
    GObjects* GObjects::s_Instance = nullptr;

    GObjects& GObjects::Get()
    {
        if (!s_Instance)
        {
            s_Instance = new GObjects();
        }
        return *s_Instance;
    }

    UObject* GObjects::CreateObject(const FString& ClassName, UObject* Outer, const FString& Name)
    {
        UClass* Class = FindClass(ClassName);
        if (!Class)
        {
            // Create a default class if it doesn't exist
            Class = RegisterClass(ClassName);
        }

        UObject* NewObject = Class->CreateObject(Outer, Name);
        if (NewObject)
        {
            RegisterObject(NewObject);
        }
        return NewObject;
    }

    void GObjects::DestroyObject(UObject* Object)
    {
        if (!Object) return;
        
        UnregisterObject(Object);
        Object->MarkPendingKill();
        Object->FinishDestroy();
        
        // Find and remove from objects map
        for (auto it = m_Objects.begin(); it != m_Objects.end(); ++it)
        {
            if (it->second.get() == Object)
            {
                m_Objects.erase(it);
                break;
            }
        }
    }

    void GObjects::DestroyObject(uint32 ObjectID)
    {
        auto it = m_Objects.find(ObjectID);
        if (it != m_Objects.end())
        {
            DestroyObject(it->second.get());
        }
    }

    UObject* GObjects::FindObject(const FString& Name) const
    {
        for (const auto& pair : m_Objects)
        {
            if (pair.second->GetName() == Name)
            {
                return pair.second.get();
            }
        }
        return nullptr;
    }

    UObject* GObjects::FindObject(uint32 ObjectID) const
    {
        auto it = m_Objects.find(ObjectID);
        return it != m_Objects.end() ? it->second.get() : nullptr;
    }

    TArray<UObject*> GObjects::FindObjectsOfClass(UClass* Class) const
    {
        TArray<UObject*> Objects;
        for (const auto& pair : m_Objects)
        {
            if (pair.second->GetClass() == Class)
            {
                Objects.Add(pair.second.get());
            }
        }
        return Objects;
    }

    TArray<UObject*> GObjects::FindObjectsOfClass(const FString& ClassName) const
    {
        TArray<UObject*> Objects;
        for (const auto& pair : m_Objects)
        {
            if (pair.second->GetClass() && pair.second->GetClass()->GetClassName() == ClassName)
            {
                Objects.Add(pair.second.get());
            }
        }
        return Objects;
    }

    TArray<UObject*> GObjects::GetAllObjects() const
    {
        TArray<UObject*> Objects;
        for (const auto& pair : m_Objects)
        {
            Objects.Add(pair.second.get());
        }
        return Objects;
    }

    void GObjects::RegisterObject(UObject* Object)
    {
        if (!Object) return;
        
        m_Objects[Object->GetObjectID()] = std::unique_ptr<UObject>(Object);
    }

    void GObjects::UnregisterObject(UObject* Object)
    {
        if (!Object) return;
        
        // Remove from pending kill objects if present
        m_PendingKillObjects.Remove(Object);
    }

    void GObjects::MarkForGarbageCollection(UObject* Object)
    {
        if (Object)
        {
            m_PendingKillObjects.Add(Object);
        }
    }

    void GObjects::CollectGarbage()
    {
        TArray<UObject*> ToDestroy;
        for (UObject* Object : m_PendingKillObjects)
        {
            ToDestroy.Add(Object);
        }
        
        for (UObject* Object : ToDestroy)
        {
            DestroyObject(Object);
        }
        
        m_PendingKillObjects.Empty();
    }

    void GObjects::ForceGarbageCollection()
    {
        CollectGarbage();
    }

    int32 GObjects::GetObjectCountOfClass(UClass* Class) const
    {
        return FindObjectsOfClass(Class).Num();
    }

    int32 GObjects::GetObjectCountOfClass(const FString& ClassName) const
    {
        return FindObjectsOfClass(ClassName).Num();
    }

    UClass* GObjects::FindClass(const FString& ClassName) const
    {
        auto it = m_Classes.find(ClassName);
        return it != m_Classes.end() ? it->second.get() : nullptr;
    }

    UClass* GObjects::RegisterClass(const FString& ClassName, UClass* SuperClass)
    {
        UClass* ExistingClass = FindClass(ClassName);
        if (ExistingClass)
        {
            return ExistingClass;
        }

        UClass* NewClass = new UClass(ClassName, SuperClass);
        m_Classes[ClassName] = std::unique_ptr<UClass>(NewClass);
        return NewClass;
    }

    TArray<UClass*> GObjects::GetAllClasses() const
    {
        TArray<UClass*> Classes;
        for (const auto& pair : m_Classes)
        {
            Classes.Add(pair.second.get());
        }
        return Classes;
    }

    void GObjects::ForEachObject(std::function<void(UObject*)> Function)
    {
        for (const auto& pair : m_Objects)
        {
            Function(pair.second.get());
        }
    }

    void GObjects::ForEachObjectOfClass(UClass* Class, std::function<void(UObject*)> Function)
    {
        for (const auto& pair : m_Objects)
        {
            if (pair.second->GetClass() == Class)
            {
                Function(pair.second.get());
            }
        }
    }

    void GObjects::Cleanup()
    {
        // Destroy all objects
        for (auto& pair : m_Objects)
        {
            pair.second->BeginDestroy();
            pair.second->FinishDestroy();
        }
        m_Objects.clear();
        
        // Clear classes
        m_Classes.clear();
        
        // Clear pending kill objects
        m_PendingKillObjects.Empty();
    }
}
