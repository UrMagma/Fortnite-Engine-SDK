# UE4 SDK - Self-Contained Unreal Engine 4.26 SDK

A clean, "vibe coder" style SDK for Unreal Engine 4.26 that provides a **self-contained** interface to UE4 functionality. **No Runtime folder needed!** Just drop this entire folder into any project and it works! ✨

## 🚀 What Makes This Special

- **Self-Contained**: All UE4 types (FVector, FRotator, TArray, etc.) are embedded directly in the SDK
- **No Dependencies**: Works without the Runtime folder - just include and go!
- **Clean API**: Simple, intuitive interface that's easy to use
- **Type Safe**: Strong typing with proper error handling
- **Well Documented**: Clear documentation and examples
- **Performance**: Optimized for performance with minimal overhead
- **Extensible**: Easy to extend with new functionality

## 📦 What's Included

```
UE4SDK/
├── UE4SDK.h              # Main SDK header
├── UE4SDK.cpp            # Main SDK implementation
├── Core/
│   ├── Types.h/.cpp      # Embedded UE4 types (FVector, FRotator, TArray, etc.)
│   ├── Utils.h/.cpp      # Utility functions
│   ├── UObject.h/.cpp    # UObject system with GObjects
│   ├── AI.h/.cpp         # AI system (AIController, BehaviorTree, etc.)
│   ├── Navigation.h/.cpp # Navigation system (NavMesh, NavPath, etc.)
│   ├── Blackboard.h/.cpp # Blackboard system
│   └── Replication.h/.cpp # Replication system with ServerReplication
├── Examples/
│   ├── SelfContainedExample.cpp    # Basic examples
│   ├── AIAndNavigationExample.cpp  # AI and Navigation examples
│   └── CompleteSystemExample.cpp   # Complete system demonstration
└── README.md             # This file
```

## 🎯 Quick Start

### 1. Include the SDK

```cpp
#include "UE4SDK.h"
```

### 2. Initialize the SDK

```cpp
UE4SDK::SDK& sdk = UE4SDK::SDK::Get();
sdk.Initialize();
```

## 🧠 System Overview

### Core Types System
- **FVector, FRotator, FTransform, FQuat**: Full math operations and transformations
- **TArray, TMap, TSet**: Container types with UE4-style API
- **FString, FName**: String handling with utility functions
- **FLinearColor, FColor**: Color types with conversion utilities

### UObject System
- **UObject**: Base object class with properties, tags, and custom data
- **UClass**: Class definition system with inheritance
- **GObjects**: Global object manager with creation, destruction, and queries

### AI System
- **AIController**: AI controller with blackboard and behavior tree integration
- **BehaviorTree**: Behavior tree system with nodes and execution
- **AITask, AIDecorator, AIService**: AI components for complex behaviors
- **AISystem**: Global AI system management

### Navigation System
- **NavMesh**: Navigation mesh with polygon support
- **NavPath**: Path representation with distance calculations
- **NavQueryFilter**: Query filters for different agent types
- **NavigationSystem**: Global navigation system management

### Blackboard System
- **BlackboardData**: Defines blackboard structure with typed keys
- **Blackboard**: Runtime blackboard instance with value storage
- **BlackboardComponent**: Component for actors to use blackboards
- **Value Change Callbacks**: Notifications when blackboard values change

### Replication System
- **ReplicationManager**: Manages object replication with conditions
- **NetworkManager**: Handles network connections and data transmission
- **ServerReplication**: Server-side replication with ReplicateActors
- **ReplicationSystem**: Global replication system coordination

### 3. Use the SDK

```cpp
// Get the engine and utils
auto& Engine = UE4_ENGINE;
auto& Utils = UE4_UTILS;

// Create vectors and do math
FVector Start = FVector(0, 0, 0);
FVector End = FVector(100, 100, 100);
float Distance = Utils.Distance(Start, End);

// Work with arrays
TArray<FVector> VectorArray;
VectorArray.Add(FVector(1, 2, 3));
VectorArray.Add(FVector(4, 5, 6));

// Work with maps
TMap<FString, int32> ScoreMap;
ScoreMap.Add(TEXT("Player1"), 100);
ScoreMap.Add(TEXT("Player2"), 200);

// String operations
FString Message = Utils.FormatString(TEXT("Hello %s!"), TEXT("World"));
```

## 📚 Embedded Types

The SDK includes all the essential UE4 types:

### Core Types
- `FString` - String handling
- `FName` - Name references
- `FVector` - 3D vectors
- `FRotator` - Rotations
- `FTransform` - Transforms
- `FQuat` - Quaternions
- `FLinearColor` - Colors
- `FColor` - 8-bit colors

### Containers
- `TArray<T>` - Dynamic arrays
- `TMap<K, V>` - Key-value maps
- `TSet<T>` - Sets

### Enums
- `ELogVerbosity` - Logging levels
- `ECollisionChannel` - Collision channels
- `EViewModeIndex` - View modes
- And many more...

## 🛠️ Core Features

### Math Operations
```cpp
// Vector math
FVector A = FVector(1, 2, 3);
FVector B = FVector(4, 5, 6);
FVector Sum = A + B;
float Dot = A | B;  // Dot product
FVector Cross = A ^ B;  // Cross product

// Rotations
FRotator Rot = FRotator(0, 45, 0);
FVector Forward = Rot.GetForwardVector();

// Transforms
FTransform Transform = FTransform(Rot, A, FVector::OneVector);
FVector Transformed = Transform.TransformPosition(FVector::ZeroVector);
```

### Container Operations
```cpp
// Arrays
TArray<int32> Numbers;
Numbers.Add(1);
Numbers.Add(2);
Numbers.Add(3);

// Filter even numbers
auto EvenNumbers = Utils.Filter(Numbers, [](int32 N) { return N % 2 == 0; });

// Map to strings
auto Strings = Utils.Map(Numbers, [](int32 N) { return Utils.ToString(N); });

// Maps
TMap<FString, int32> Scores;
Scores.Add(TEXT("Player1"), 100);
int32 Score = Scores[TEXT("Player1")];

// Sets
TSet<FString> Tags;
Tags.Add(TEXT("Player"));
bool bHasTag = Tags.Contains(TEXT("Player"));
```

### String Operations
```cpp
FString Text = TEXT("Hello,World,UE4");
auto Parts = Utils.SplitString(Text, TEXT(","));
FString Joined = Utils.JoinString(Parts, TEXT(" | "));

FString Formatted = Utils.FormatString(TEXT("Player %s has %d points"), TEXT("VibeCoder"), 1000);
```

### Utility Functions
```cpp
// Math utilities
float Lerped = Utils.Lerp(0.0f, 100.0f, 0.5f);
float Clamped = Utils.Clamp(150.0f, 0.0f, 100.0f);
float Mapped = Utils.MapRange(50.0f, 0.0f, 100.0f, 0.0f, 1.0f);

// Random utilities
float RandomFloat = Utils.RandomFloat(0.0f, 100.0f);
FVector RandomVector = Utils.RandomVector(FVector(-100, -100, -100), FVector(100, 100, 100));

// Distance calculations
float Distance = Utils.Distance(Start, End);
FVector Direction = Utils.Direction(Start, End);
```

### Performance Timing
```cpp
// Start timer
Utils.StartTimer(TEXT("MyOperation"));

// Do work
for (int32 i = 0; i < 1000; ++i)
{
    // Some operation
}

// Stop timer and get elapsed time
float Elapsed = Utils.StopTimer(TEXT("MyOperation"));
UE_LOG(LogUE4SDK, Log, TEXT("Operation took %s seconds"), *Utils.ToString(Elapsed));
```

### File Operations
```cpp
// Save and load strings
FString Data = TEXT("Hello, UE4 SDK!");
Utils.SaveStringToFile(Data, TEXT("test.txt"));
FString Loaded = Utils.LoadStringFromFile(TEXT("test.txt"));

// File system operations
bool bExists = Utils.FileExists(TEXT("test.txt"));
bool bCreated = Utils.CreateDirectory(TEXT("MyFolder"));
auto Files = Utils.GetFilesInDirectory(TEXT("MyFolder"), TEXT(".txt"));
```

### Event System
```cpp
// Bind events
Utils.BindEvent(TEXT("PlayerDied"), []()
{
    UE_LOG(LogUE4SDK, Log, TEXT("Player died!"));
});

// Trigger events
Utils.TriggerEvent(TEXT("PlayerDied"));
```

## 🎮 Game Development Examples

### Player Management
```cpp
struct Player
{
    FString Name;
    int32 Score;
    FVector Position;
    bool bAlive;
};

TArray<Player> Players;
Players.Add(Player(TEXT("VibeCoder"), 1000, FVector(0, 0, 0)));

// Find highest scoring player
Player* BestPlayer = nullptr;
int32 HighestScore = 0;
for (auto& Player : Players)
{
    if (Player.Score > HighestScore)
    {
        HighestScore = Player.Score;
        BestPlayer = &Player;
    }
}
```

### Distance Calculations
```cpp
// Calculate distances between all players
for (int32 i = 0; i < Players.Num(); ++i)
{
    for (int32 j = i + 1; j < Players.Num(); ++j)
    {
        float Distance = Utils.Distance(Players[i].Position, Players[j].Position);
        UE_LOG(LogUE4SDK, Log, TEXT("Distance: %s"), *Utils.ToString(Distance));
    }
}
```

### Leaderboard
```cpp
// Sort players by score
TArray<Player*> SortedPlayers;
for (auto& Player : Players)
{
    SortedPlayers.Add(&Player);
}

SortedPlayers.Sort([](const Player* A, const Player* B) { return A->Score > B->Score; });

// Display leaderboard
for (int32 i = 0; i < SortedPlayers.Num(); ++i)
{
    UE_LOG(LogUE4SDK, Log, TEXT("%d. %s - %d points"), 
           i + 1, *SortedPlayers[i]->Name, SortedPlayers[i]->Score);
}
```

## 🔧 Building

### Prerequisites
- C++11 compatible compiler
- No external dependencies!

### Build Steps
1. Copy the entire `UE4SDK` folder to your project
2. Include `UE4SDK.h` in your source files
3. Compile and run!

### Example Build Command
```bash
g++ -std=c++11 -I./UE4SDK main.cpp UE4SDK/UE4SDK.cpp -o mygame
```

## 📖 API Reference

### Core Types
| Type | Description |
|------|-------------|
| `FVector` | 3D vector with X, Y, Z components |
| `FRotator` | Rotation with Pitch, Yaw, Roll |
| `FTransform` | Transform with rotation, translation, scale |
| `FString` | String class with UE4-style methods |
| `TArray<T>` | Dynamic array container |
| `TMap<K,V>` | Key-value map container |
| `TSet<T>` | Set container |

### Utils Functions
| Function | Description |
|----------|-------------|
| `Distance(A, B)` | Calculate distance between vectors |
| `Lerp(A, B, Alpha)` | Linear interpolation |
| `RandomFloat(Min, Max)` | Generate random float |
| `FormatString(Format, ...)` | Format string with parameters |
| `SaveStringToFile(String, Path)` | Save string to file |
| `StartTimer(Name)` | Start performance timer |
| `StopTimer(Name)` | Stop timer and get elapsed time |

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 🙏 Acknowledgments

- Epic Games for Unreal Engine 4
- The UE4 community for inspiration and feedback

## 📞 Support

If you have any questions or need help:

1. Check the examples in `Examples/SelfContainedExample.cpp`
2. Look at the embedded type definitions in `Core/Types.h`
3. Create an issue if you need help

---

**Made with ❤️ for developers who love clean, self-contained code**

**Drop it in, include it, and start coding! No Runtime folder needed!** 🚀