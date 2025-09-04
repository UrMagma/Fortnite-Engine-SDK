/*
 * UE4 Blackboard Implementation - Self-contained Blackboard system implementation
 * Vibe coder approved ✨
 */

#include "Blackboard.h"
#include "Utils.h"
#include <algorithm>

namespace UE4SDK
{
    // BlackboardKey implementation
    BlackboardKey::BlackboardKey(const FString& InKeyName, EBlackboardKeyType InKeyType)
        : m_KeyName(InKeyName), m_KeyType(InKeyType)
    {
    }

    bool BlackboardKey::operator==(const BlackboardKey& Other) const
    {
        return m_KeyName == Other.m_KeyName && m_KeyType == Other.m_KeyType;
    }

    bool BlackboardKey::operator!=(const BlackboardKey& Other) const
    {
        return !(*this == Other);
    }

    // BlackboardData implementation
    BlackboardData::BlackboardData()
        : m_DataName(TEXT("UnnamedBlackboardData"))
    {
    }

    void BlackboardData::AddKey(const FString& KeyName, EBlackboardKeyType KeyType, const FString& Description)
    {
        if (HasKey(KeyName))
        {
            Utils::Get().LogWarning(TEXT("BlackboardData::AddKey - Key already exists: ") + KeyName);
            return;
        }

        auto NewKey = std::make_unique<BlackboardKey>(KeyName, KeyType);
        NewKey->SetDescription(Description);
        
        m_KeyMap.Add(KeyName, NewKey.get());
        m_Keys.Add(std::move(NewKey));
    }

    void BlackboardData::RemoveKey(const FString& KeyName)
    {
        auto* Key = m_KeyMap.Find(KeyName);
        if (Key)
        {
            m_KeyMap.Remove(KeyName);
            
            // Remove from keys array
            for (auto it = m_Keys.begin(); it != m_Keys.end(); ++it)
            {
                if (it->get() == *Key)
                {
                    m_Keys.erase(it);
                    break;
                }
            }
        }
    }

    bool BlackboardData::HasKey(const FString& KeyName) const
    {
        return m_KeyMap.Contains(KeyName);
    }

    BlackboardKey* BlackboardData::GetKey(const FString& KeyName) const
    {
        auto* Key = m_KeyMap.Find(KeyName);
        return Key ? *Key : nullptr;
    }

    TArray<BlackboardKey*> BlackboardData::GetAllKeys() const
    {
        TArray<BlackboardKey*> Keys;
        for (const auto& key : m_Keys)
        {
            Keys.Add(key.get());
        }
        return Keys;
    }

    TArray<FString> BlackboardData::GetKeyNames() const
    {
        TArray<FString> KeyNames;
        for (const auto& pair : m_KeyMap)
        {
            KeyNames.Add(pair.first);
        }
        return KeyNames;
    }

    EBlackboardKeyType BlackboardData::GetKeyType(const FString& KeyName) const
    {
        auto* Key = GetKey(KeyName);
        return Key ? Key->GetKeyType() : EBlackboardKeyType::Invalid;
    }

    bool BlackboardData::IsKeyOfType(const FString& KeyName, EBlackboardKeyType KeyType) const
    {
        return GetKeyType(KeyName) == KeyType;
    }

    bool BlackboardData::IsValid() const
    {
        return !m_DataName.IsEmpty() && m_Keys.Num() > 0;
    }

    TArray<FString> BlackboardData::ValidateData() const
    {
        TArray<FString> Errors;
        
        if (m_DataName.IsEmpty())
        {
            Errors.Add(TEXT("BlackboardData name is empty"));
        }
        
        if (m_Keys.Num() == 0)
        {
            Errors.Add(TEXT("BlackboardData has no keys"));
        }
        
        // Check for duplicate key names
        TSet<FString> KeyNames;
        for (const auto& key : m_Keys)
        {
            if (KeyNames.Contains(key->GetKeyName()))
            {
                Errors.Add(TEXT("Duplicate key name: ") + key->GetKeyName());
            }
            else
            {
                KeyNames.Add(key->GetKeyName());
            }
        }
        
        return Errors;
    }

    FString BlackboardData::SerializeData() const
    {
        FString Result = TEXT("BlackboardData:") + m_DataName + TEXT("\n");
        Result += TEXT("Keys:\n");
        
        for (const auto& key : m_Keys)
        {
            Result += TEXT("  ") + key->GetKeyName() + TEXT(":") + FString::FromInt(static_cast<int32>(key->GetKeyType()));
            if (!key->GetDescription().IsEmpty())
            {
                Result += TEXT(" // ") + key->GetDescription();
            }
            Result += TEXT("\n");
        }
        
        return Result;
    }

    bool BlackboardData::DeserializeData(const FString& Data)
    {
        // Simple deserialization - in a real implementation this would be more robust
        auto Lines = Data.Split(TEXT("\n"));
        bool InKeys = false;
        
        for (const auto& Line : Lines)
        {
            FString TrimmedLine = Line.Trim();
            if (TrimmedLine.StartsWith(TEXT("BlackboardData:")))
            {
                m_DataName = TrimmedLine.Mid(15);
            }
            else if (TrimmedLine == TEXT("Keys:"))
            {
                InKeys = true;
            }
            else if (InKeys && TrimmedLine.StartsWith(TEXT("  ")))
            {
                FString KeyLine = TrimmedLine.Mid(2);
                int32 ColonIndex = KeyLine.Find(TEXT(":"));
                if (ColonIndex > 0)
                {
                    FString KeyName = KeyLine.Left(ColonIndex);
                    FString TypeStr = KeyLine.Mid(ColonIndex + 1);
                    
                    // Remove comment if present
                    int32 CommentIndex = TypeStr.Find(TEXT("//"));
                    if (CommentIndex > 0)
                    {
                        TypeStr = TypeStr.Left(CommentIndex).Trim();
                    }
                    
                    int32 TypeValue = TypeStr.ToInt();
                    if (TypeValue >= 0 && TypeValue <= 10)
                    {
                        AddKey(KeyName, static_cast<EBlackboardKeyType>(TypeValue));
                    }
                }
            }
        }
        
        return true;
    }

    // Blackboard implementation
    Blackboard::Blackboard()
        : m_BlackboardName(TEXT("UnnamedBlackboard")), m_BlackboardData(nullptr)
    {
    }

    void Blackboard::SetBlackboardData(BlackboardData* InData)
    {
        m_BlackboardData = InData;
    }

    void Blackboard::SetValueAsObject(const FString& KeyName, UObject* ObjectValue)
    {
        if (!IsKeyRegistered(KeyName) || GetRegisteredKeyType(KeyName) != EBlackboardKeyType::Object)
        {
            Utils::Get().LogWarning(TEXT("Blackboard::SetValueAsObject - Invalid key or type: ") + KeyName);
            return;
        }
        
        m_ObjectValues.Add(KeyName, ObjectValue);
        NotifyValueChanged(KeyName);
    }

    void Blackboard::SetValueAsClass(const FString& KeyName, UClass* ClassValue)
    {
        if (!IsKeyRegistered(KeyName) || GetRegisteredKeyType(KeyName) != EBlackboardKeyType::Class)
        {
            Utils::Get().LogWarning(TEXT("Blackboard::SetValueAsClass - Invalid key or type: ") + KeyName);
            return;
        }
        
        m_ClassValues.Add(KeyName, ClassValue);
        NotifyValueChanged(KeyName);
    }

    void Blackboard::SetValueAsEnum(const FString& KeyName, int32 EnumValue)
    {
        if (!IsKeyRegistered(KeyName) || GetRegisteredKeyType(KeyName) != EBlackboardKeyType::Enum)
        {
            Utils::Get().LogWarning(TEXT("Blackboard::SetValueAsEnum - Invalid key or type: ") + KeyName);
            return;
        }
        
        m_IntValues.Add(KeyName, EnumValue);
        NotifyValueChanged(KeyName);
    }

    void Blackboard::SetValueAsInt(const FString& KeyName, int32 IntValue)
    {
        if (!IsKeyRegistered(KeyName) || GetRegisteredKeyType(KeyName) != EBlackboardKeyType::Int)
        {
            Utils::Get().LogWarning(TEXT("Blackboard::SetValueAsInt - Invalid key or type: ") + KeyName);
            return;
        }
        
        m_IntValues.Add(KeyName, IntValue);
        NotifyValueChanged(KeyName);
    }

    void Blackboard::SetValueAsFloat(const FString& KeyName, float FloatValue)
    {
        if (!IsKeyRegistered(KeyName) || GetRegisteredKeyType(KeyName) != EBlackboardKeyType::Float)
        {
            Utils::Get().LogWarning(TEXT("Blackboard::SetValueAsFloat - Invalid key or type: ") + KeyName);
            return;
        }
        
        m_FloatValues.Add(KeyName, FloatValue);
        NotifyValueChanged(KeyName);
    }

    void Blackboard::SetValueAsBool(const FString& KeyName, bool BoolValue)
    {
        if (!IsKeyRegistered(KeyName) || GetRegisteredKeyType(KeyName) != EBlackboardKeyType::Bool)
        {
            Utils::Get().LogWarning(TEXT("Blackboard::SetValueAsBool - Invalid key or type: ") + KeyName);
            return;
        }
        
        m_BoolValues.Add(KeyName, BoolValue);
        NotifyValueChanged(KeyName);
    }

    void Blackboard::SetValueAsString(const FString& KeyName, const FString& StringValue)
    {
        if (!IsKeyRegistered(KeyName) || GetRegisteredKeyType(KeyName) != EBlackboardKeyType::String)
        {
            Utils::Get().LogWarning(TEXT("Blackboard::SetValueAsString - Invalid key or type: ") + KeyName);
            return;
        }
        
        m_StringValues.Add(KeyName, StringValue);
        NotifyValueChanged(KeyName);
    }

    void Blackboard::SetValueAsVector(const FString& KeyName, const FVector& VectorValue)
    {
        if (!IsKeyRegistered(KeyName) || GetRegisteredKeyType(KeyName) != EBlackboardKeyType::Vector)
        {
            Utils::Get().LogWarning(TEXT("Blackboard::SetValueAsVector - Invalid key or type: ") + KeyName);
            return;
        }
        
        m_VectorValues.Add(KeyName, VectorValue);
        NotifyValueChanged(KeyName);
    }

    void Blackboard::SetValueAsRotator(const FString& KeyName, const FRotator& RotatorValue)
    {
        if (!IsKeyRegistered(KeyName) || GetRegisteredKeyType(KeyName) != EBlackboardKeyType::Rotator)
        {
            Utils::Get().LogWarning(TEXT("Blackboard::SetValueAsRotator - Invalid key or type: ") + KeyName);
            return;
        }
        
        m_RotatorValues.Add(KeyName, RotatorValue);
        NotifyValueChanged(KeyName);
    }

    void Blackboard::SetValueAsName(const FString& KeyName, const FName& NameValue)
    {
        if (!IsKeyRegistered(KeyName) || GetRegisteredKeyType(KeyName) != EBlackboardKeyType::Name)
        {
            Utils::Get().LogWarning(TEXT("Blackboard::SetValueAsName - Invalid key or type: ") + KeyName);
            return;
        }
        
        m_NameValues.Add(KeyName, NameValue);
        NotifyValueChanged(KeyName);
    }

    UObject* Blackboard::GetValueAsObject(const FString& KeyName) const
    {
        auto* Value = m_ObjectValues.Find(KeyName);
        return Value ? *Value : nullptr;
    }

    UClass* Blackboard::GetValueAsClass(const FString& KeyName) const
    {
        auto* Value = m_ClassValues.Find(KeyName);
        return Value ? *Value : nullptr;
    }

    int32 Blackboard::GetValueAsEnum(const FString& KeyName) const
    {
        auto* Value = m_IntValues.Find(KeyName);
        return Value ? *Value : 0;
    }

    int32 Blackboard::GetValueAsInt(const FString& KeyName) const
    {
        auto* Value = m_IntValues.Find(KeyName);
        return Value ? *Value : 0;
    }

    float Blackboard::GetValueAsFloat(const FString& KeyName) const
    {
        auto* Value = m_FloatValues.Find(KeyName);
        return Value ? *Value : 0.0f;
    }

    bool Blackboard::GetValueAsBool(const FString& KeyName) const
    {
        auto* Value = m_BoolValues.Find(KeyName);
        return Value ? *Value : false;
    }

    FString Blackboard::GetValueAsString(const FString& KeyName) const
    {
        auto* Value = m_StringValues.Find(KeyName);
        return Value ? *Value : FString();
    }

    FVector Blackboard::GetValueAsVector(const FString& KeyName) const
    {
        auto* Value = m_VectorValues.Find(KeyName);
        return Value ? *Value : FVector::ZeroVector;
    }

    FRotator Blackboard::GetValueAsRotator(const FString& KeyName) const
    {
        auto* Value = m_RotatorValues.Find(KeyName);
        return Value ? *Value : FRotator::ZeroRotator;
    }

    FName Blackboard::GetValueAsName(const FString& KeyName) const
    {
        auto* Value = m_NameValues.Find(KeyName);
        return Value ? *Value : FName();
    }

    bool Blackboard::IsValidKey(const FString& KeyName) const
    {
        return IsKeyRegistered(KeyName);
    }

    bool Blackboard::HasValue(const FString& KeyName) const
    {
        return m_ObjectValues.Contains(KeyName) ||
               m_ClassValues.Contains(KeyName) ||
               m_IntValues.Contains(KeyName) ||
               m_FloatValues.Contains(KeyName) ||
               m_BoolValues.Contains(KeyName) ||
               m_StringValues.Contains(KeyName) ||
               m_VectorValues.Contains(KeyName) ||
               m_RotatorValues.Contains(KeyName) ||
               m_NameValues.Contains(KeyName);
    }

    bool Blackboard::IsValueSet(const FString& KeyName) const
    {
        return HasValue(KeyName);
    }

    void Blackboard::ClearValue(const FString& KeyName)
    {
        m_ObjectValues.Remove(KeyName);
        m_ClassValues.Remove(KeyName);
        m_IntValues.Remove(KeyName);
        m_FloatValues.Remove(KeyName);
        m_BoolValues.Remove(KeyName);
        m_StringValues.Remove(KeyName);
        m_VectorValues.Remove(KeyName);
        m_RotatorValues.Remove(KeyName);
        m_NameValues.Remove(KeyName);
        
        NotifyValueChanged(KeyName);
    }

    void Blackboard::ClearAllValues()
    {
        m_ObjectValues.Empty();
        m_ClassValues.Empty();
        m_IntValues.Empty();
        m_FloatValues.Empty();
        m_BoolValues.Empty();
        m_StringValues.Empty();
        m_VectorValues.Empty();
        m_RotatorValues.Empty();
        m_NameValues.Empty();
    }

    void Blackboard::ClearUnusedValues()
    {
        if (!m_BlackboardData) return;
        
        TArray<FString> ValidKeys = m_BlackboardData->GetKeyNames();
        
        // Remove values for keys that are no longer in the data
        TArray<FString> KeysToRemove;
        
        for (const auto& pair : m_ObjectValues)
        {
            if (!ValidKeys.Contains(pair.first))
            {
                KeysToRemove.Add(pair.first);
            }
        }
        
        for (const auto& Key : KeysToRemove)
        {
            ClearValue(Key);
        }
    }

    void Blackboard::SetOnValueChangedCallback(const FString& KeyName, std::function<void()> Callback)
    {
        m_ValueChangedCallbacks.Add(KeyName, Callback);
    }

    void Blackboard::ClearOnValueChangedCallback(const FString& KeyName)
    {
        m_ValueChangedCallbacks.Remove(KeyName);
    }

    void Blackboard::SetOnAnyValueChangedCallback(std::function<void(const FString&)> Callback)
    {
        m_AnyValueChangedCallback = Callback;
    }

    void Blackboard::Initialize()
    {
        if (m_BlackboardData)
        {
            Utils::Get().LogInfo(TEXT("Blackboard initialized: ") + m_BlackboardName);
        }
    }

    void Blackboard::Shutdown()
    {
        ClearAllValues();
        m_ValueChangedCallbacks.Empty();
        m_AnyValueChangedCallback = nullptr;
    }

    void Blackboard::ForEachValue(std::function<void(const FString&, EBlackboardKeyType)> Function) const
    {
        if (!m_BlackboardData) return;
        
        for (const auto& pair : m_ObjectValues)
        {
            Function(pair.first, EBlackboardKeyType::Object);
        }
        for (const auto& pair : m_ClassValues)
        {
            Function(pair.first, EBlackboardKeyType::Class);
        }
        for (const auto& pair : m_IntValues)
        {
            Function(pair.first, EBlackboardKeyType::Int);
        }
        for (const auto& pair : m_FloatValues)
        {
            Function(pair.first, EBlackboardKeyType::Float);
        }
        for (const auto& pair : m_BoolValues)
        {
            Function(pair.first, EBlackboardKeyType::Bool);
        }
        for (const auto& pair : m_StringValues)
        {
            Function(pair.first, EBlackboardKeyType::String);
        }
        for (const auto& pair : m_VectorValues)
        {
            Function(pair.first, EBlackboardKeyType::Vector);
        }
        for (const auto& pair : m_RotatorValues)
        {
            Function(pair.first, EBlackboardKeyType::Rotator);
        }
        for (const auto& pair : m_NameValues)
        {
            Function(pair.first, EBlackboardKeyType::Name);
        }
    }

    TArray<FString> Blackboard::GetSetKeys() const
    {
        TArray<FString> SetKeys;
        ForEachValue([&SetKeys](const FString& KeyName, EBlackboardKeyType) {
            SetKeys.Add(KeyName);
        });
        return SetKeys;
    }

    TArray<FString> Blackboard::GetUnsetKeys() const
    {
        TArray<FString> UnsetKeys;
        if (m_BlackboardData)
        {
            TArray<FString> AllKeys = m_BlackboardData->GetKeyNames();
            TArray<FString> SetKeys = GetSetKeys();
            
            for (const auto& Key : AllKeys)
            {
                if (!SetKeys.Contains(Key))
                {
                    UnsetKeys.Add(Key);
                }
            }
        }
        return UnsetKeys;
    }

    bool Blackboard::ValidateValue(const FString& KeyName, EBlackboardKeyType ExpectedType) const
    {
        if (!IsKeyRegistered(KeyName))
        {
            return false;
        }
        
        EBlackboardKeyType ActualType = GetRegisteredKeyType(KeyName);
        return ActualType == ExpectedType;
    }

    TArray<FString> Blackboard::ValidateAllValues() const
    {
        TArray<FString> Errors;
        
        if (!m_BlackboardData)
        {
            Errors.Add(TEXT("Blackboard has no data"));
            return Errors;
        }
        
        // Check all set values against their registered types
        ForEachValue([&Errors, this](const FString& KeyName, EBlackboardKeyType ActualType) {
            EBlackboardKeyType ExpectedType = GetRegisteredKeyType(KeyName);
            if (ActualType != ExpectedType)
            {
                Errors.Add(TEXT("Type mismatch for key ") + KeyName);
            }
        });
        
        return Errors;
    }

    void Blackboard::CopyFrom(const Blackboard* Other)
    {
        if (!Other) return;
        
        ClearAllValues();
        
        m_ObjectValues = Other->m_ObjectValues;
        m_ClassValues = Other->m_ClassValues;
        m_IntValues = Other->m_IntValues;
        m_FloatValues = Other->m_FloatValues;
        m_BoolValues = Other->m_BoolValues;
        m_StringValues = Other->m_StringValues;
        m_VectorValues = Other->m_VectorValues;
        m_RotatorValues = Other->m_RotatorValues;
        m_NameValues = Other->m_NameValues;
    }

    void Blackboard::CopyTo(Blackboard* Other) const
    {
        if (Other)
        {
            Other->CopyFrom(this);
        }
    }

    void Blackboard::LogBlackboardState() const
    {
        Utils::Get().LogInfo(TEXT("=== Blackboard State: ") + m_BlackboardName + TEXT(" ==="));
        
        ForEachValue([](const FString& KeyName, EBlackboardKeyType KeyType) {
            FString TypeStr = FString::FromInt(static_cast<int32>(KeyType));
            Utils::Get().LogInfo(TEXT("  ") + KeyName + TEXT(": ") + TypeStr);
        });
        
        Utils::Get().LogInfo(TEXT("=== End Blackboard State ==="));
    }

    FString Blackboard::GetBlackboardStateString() const
    {
        FString Result = TEXT("Blackboard: ") + m_BlackboardName + TEXT("\n");
        
        ForEachValue([&Result](const FString& KeyName, EBlackboardKeyType KeyType) {
            Result += TEXT("  ") + KeyName + TEXT(": ") + FString::FromInt(static_cast<int32>(KeyType)) + TEXT("\n");
        });
        
        return Result;
    }

    void Blackboard::NotifyValueChanged(const FString& KeyName)
    {
        // Call specific key callback
        auto* Callback = m_ValueChangedCallbacks.Find(KeyName);
        if (Callback)
        {
            (*Callback)();
        }
        
        // Call any value changed callback
        if (m_AnyValueChangedCallback)
        {
            m_AnyValueChangedCallback(KeyName);
        }
    }

    bool Blackboard::IsKeyRegistered(const FString& KeyName) const
    {
        return m_BlackboardData && m_BlackboardData->HasKey(KeyName);
    }

    EBlackboardKeyType Blackboard::GetRegisteredKeyType(const FString& KeyName) const
    {
        return m_BlackboardData ? m_BlackboardData->GetKeyType(KeyName) : EBlackboardKeyType::Invalid;
    }

    // BlackboardComponent implementation
    BlackboardComponent::BlackboardComponent()
        : m_ComponentName(TEXT("UnnamedBlackboardComponent")), m_Owner(nullptr), m_Blackboard(nullptr)
    {
    }

    void BlackboardComponent::SetBlackboard(Blackboard* InBlackboard)
    {
        m_Blackboard = InBlackboard;
    }

    void BlackboardComponent::SetValueAsObject(const FString& KeyName, UObject* ObjectValue)
    {
        if (m_Blackboard)
        {
            m_Blackboard->SetValueAsObject(KeyName, ObjectValue);
        }
    }

    void BlackboardComponent::SetValueAsInt(const FString& KeyName, int32 IntValue)
    {
        if (m_Blackboard)
        {
            m_Blackboard->SetValueAsInt(KeyName, IntValue);
        }
    }

    void BlackboardComponent::SetValueAsFloat(const FString& KeyName, float FloatValue)
    {
        if (m_Blackboard)
        {
            m_Blackboard->SetValueAsFloat(KeyName, FloatValue);
        }
    }

    void BlackboardComponent::SetValueAsBool(const FString& KeyName, bool BoolValue)
    {
        if (m_Blackboard)
        {
            m_Blackboard->SetValueAsBool(KeyName, BoolValue);
        }
    }

    void BlackboardComponent::SetValueAsString(const FString& KeyName, const FString& StringValue)
    {
        if (m_Blackboard)
        {
            m_Blackboard->SetValueAsString(KeyName, StringValue);
        }
    }

    void BlackboardComponent::SetValueAsVector(const FString& KeyName, const FVector& VectorValue)
    {
        if (m_Blackboard)
        {
            m_Blackboard->SetValueAsVector(KeyName, VectorValue);
        }
    }

    void BlackboardComponent::SetValueAsRotator(const FString& KeyName, const FRotator& RotatorValue)
    {
        if (m_Blackboard)
        {
            m_Blackboard->SetValueAsRotator(KeyName, RotatorValue);
        }
    }

    UObject* BlackboardComponent::GetValueAsObject(const FString& KeyName) const
    {
        return m_Blackboard ? m_Blackboard->GetValueAsObject(KeyName) : nullptr;
    }

    int32 BlackboardComponent::GetValueAsInt(const FString& KeyName) const
    {
        return m_Blackboard ? m_Blackboard->GetValueAsInt(KeyName) : 0;
    }

    float BlackboardComponent::GetValueAsFloat(const FString& KeyName) const
    {
        return m_Blackboard ? m_Blackboard->GetValueAsFloat(KeyName) : 0.0f;
    }

    bool BlackboardComponent::GetValueAsBool(const FString& KeyName) const
    {
        return m_Blackboard ? m_Blackboard->GetValueAsBool(KeyName) : false;
    }

    FString BlackboardComponent::GetValueAsString(const FString& KeyName) const
    {
        return m_Blackboard ? m_Blackboard->GetValueAsString(KeyName) : FString();
    }

    FVector BlackboardComponent::GetValueAsVector(const FString& KeyName) const
    {
        return m_Blackboard ? m_Blackboard->GetValueAsVector(KeyName) : FVector::ZeroVector;
    }

    FRotator BlackboardComponent::GetValueAsRotator(const FString& KeyName) const
    {
        return m_Blackboard ? m_Blackboard->GetValueAsRotator(KeyName) : FRotator::ZeroRotator;
    }

    void BlackboardComponent::Initialize()
    {
        if (m_Blackboard)
        {
            m_Blackboard->Initialize();
        }
    }

    void BlackboardComponent::Shutdown()
    {
        if (m_Blackboard)
        {
            m_Blackboard->Shutdown();
        }
    }

    // BlackboardSystem implementation
    BlackboardSystem* BlackboardSystem::s_Instance = nullptr;

    BlackboardSystem& BlackboardSystem::Get()
    {
        if (!s_Instance)
        {
            s_Instance = new BlackboardSystem();
        }
        return *s_Instance;
    }

    BlackboardData* BlackboardSystem::CreateBlackboardData(const FString& DataName)
    {
        auto NewData = std::make_unique<BlackboardData>();
        NewData->SetDataName(DataName);
        BlackboardData* DataPtr = NewData.get();
        m_BlackboardData.Add(std::move(NewData));
        return DataPtr;
    }

    void BlackboardSystem::DestroyBlackboardData(BlackboardData* Data)
    {
        for (auto it = m_BlackboardData.begin(); it != m_BlackboardData.end(); ++it)
        {
            if (it->get() == Data)
            {
                m_BlackboardData.erase(it);
                break;
            }
        }
    }

    BlackboardData* BlackboardSystem::FindBlackboardData(const FString& DataName) const
    {
        for (const auto& data : m_BlackboardData)
        {
            if (data->GetDataName() == DataName)
            {
                return data.get();
            }
        }
        return nullptr;
    }

    TArray<BlackboardData*> BlackboardSystem::GetAllBlackboardData() const
    {
        TArray<BlackboardData*> DataArray;
        for (const auto& data : m_BlackboardData)
        {
            DataArray.Add(data.get());
        }
        return DataArray;
    }

    Blackboard* BlackboardSystem::CreateBlackboard(const FString& BlackboardName, BlackboardData* Data)
    {
        auto NewBlackboard = std::make_unique<Blackboard>();
        NewBlackboard->SetBlackboardName(BlackboardName);
        if (Data)
        {
            NewBlackboard->SetBlackboardData(Data);
        }
        Blackboard* BlackboardPtr = NewBlackboard.get();
        m_Blackboards.Add(std::move(NewBlackboard));
        return BlackboardPtr;
    }

    void BlackboardSystem::DestroyBlackboard(Blackboard* Blackboard)
    {
        for (auto it = m_Blackboards.begin(); it != m_Blackboards.end(); ++it)
        {
            if (it->get() == Blackboard)
            {
                it->get()->Shutdown();
                m_Blackboards.erase(it);
                break;
            }
        }
    }

    Blackboard* BlackboardSystem::FindBlackboard(const FString& BlackboardName) const
    {
        for (const auto& blackboard : m_Blackboards)
        {
            if (blackboard->GetBlackboardName() == BlackboardName)
            {
                return blackboard.get();
            }
        }
        return nullptr;
    }

    TArray<Blackboard*> BlackboardSystem::GetAllBlackboards() const
    {
        TArray<Blackboard*> BlackboardArray;
        for (const auto& blackboard : m_Blackboards)
        {
            BlackboardArray.Add(blackboard.get());
        }
        return BlackboardArray;
    }

    BlackboardComponent* BlackboardSystem::CreateBlackboardComponent(const FString& ComponentName)
    {
        auto NewComponent = std::make_unique<BlackboardComponent>();
        NewComponent->SetComponentName(ComponentName);
        BlackboardComponent* ComponentPtr = NewComponent.get();
        m_BlackboardComponents.Add(std::move(NewComponent));
        return ComponentPtr;
    }

    void BlackboardSystem::DestroyBlackboardComponent(BlackboardComponent* Component)
    {
        for (auto it = m_BlackboardComponents.begin(); it != m_BlackboardComponents.end(); ++it)
        {
            if (it->get() == Component)
            {
                it->get()->Shutdown();
                m_BlackboardComponents.erase(it);
                break;
            }
        }
    }

    BlackboardComponent* BlackboardSystem::FindBlackboardComponent(const FString& ComponentName) const
    {
        for (const auto& component : m_BlackboardComponents)
        {
            if (component->GetComponentName() == ComponentName)
            {
                return component.get();
            }
        }
        return nullptr;
    }

    TArray<BlackboardComponent*> BlackboardSystem::GetAllBlackboardComponents() const
    {
        TArray<BlackboardComponent*> ComponentArray;
        for (const auto& component : m_BlackboardComponents)
        {
            ComponentArray.Add(component.get());
        }
        return ComponentArray;
    }

    int32 BlackboardSystem::GetBlackboardDataCount() const
    {
        return static_cast<int32>(m_BlackboardData.Num());
    }

    int32 BlackboardSystem::GetBlackboardCount() const
    {
        return static_cast<int32>(m_Blackboards.Num());
    }

    int32 BlackboardSystem::GetBlackboardComponentCount() const
    {
        return static_cast<int32>(m_BlackboardComponents.Num());
    }

    void BlackboardSystem::Initialize()
    {
        Utils::Get().LogInfo(TEXT("BlackboardSystem initialized"));
    }

    void BlackboardSystem::Shutdown()
    {
        // Shutdown all components
        for (auto& component : m_BlackboardComponents)
        {
            component->Shutdown();
        }
        
        // Shutdown all blackboards
        for (auto& blackboard : m_Blackboards)
        {
            blackboard->Shutdown();
        }
        
        Utils::Get().LogInfo(TEXT("BlackboardSystem shutdown"));
    }

    void BlackboardSystem::Cleanup()
    {
        m_BlackboardComponents.clear();
        m_Blackboards.clear();
        m_BlackboardData.clear();
    }
}
