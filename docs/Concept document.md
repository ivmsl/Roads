# City Builder Game - Conceptual Document & Development Guide

## Project Overview

### Vision

A city builder game prototype focused on road network design and traffic simulation, inspired by Cities Skylines and SimCity. The project emphasizes road placement mechanics and traffic AI over complex city management systems.

### Core Philosophy

- **Simplicity First**: Prioritize functional mechanics over visual complexity
- **Iterative Development**: Build features incrementally with future expansion in mind
- **Learning Focus**: Design systems that allow experimentation with AI and pathfinding

## Technical Specifications

### Platform & Tools

- **Language**: C++
- **Framework**: Raylib
- **Platform**: macOS (primary), with potential cross-platform compatibility
- **IDE**: VS Code
- **Performance Target**: Smooth gameplay (60 FPS target)

### Visual Design

- **Perspective**: Perspective 3D view with 60-70° tilt angle
- **Art Style**: Primitive geometric shapes with solid colors initially
- **Grid System**: 10x10 meter tiles
- **Visual Priority**: Clear traffic flow visualization over aesthetic detail

## Project Scope

### Primary Features (MVP)

1. **Map System**

   - Fixed-size map (100x100 tiles initially)
   - Flat terrain (elevation for future consideration)
   - Grid-based tile system
2. **Road System**

   - Click-and-drag placement with grid snapping
   - Initial support for horizontal/vertical roads
   - Road types: Generic (one-way initially), expandable to multiple types
   - Intersection detection: Forms only when:
     - A new road is started by clicking on an existing road
     - A road is dragged to end on an existing road
     - Roads do NOT auto-connect when placed adjacent
3. **Traffic System**

   - Basic "follow the road" AI
   - Lane maintenance
   - Yield-to-right at intersections
   - Variable vehicle speeds
   - Traffic spawn objects for vehicle generation
4. **Camera System**

   - Perspective view controls
   - Pan, zoom functionality
   - Grid visibility toggle

### Secondary Features

1. **Extended Road Types**

   - Gravel roads, two-way roads, four-lane roads, highways
   - Diagonal road placement
   - Curved roads (future consideration)
2. **Advanced Intersections**

   - T-junctions and 4-way intersections
   - Traffic control settings (traffic lights, stop signs, yield signs)
   - Turn lane configuration
3. **Zone System**

   - Commercial, Residential, Industrial, Utility zones
   - Visual differentiation through colors
   - Traffic demand generation based on zone types
4. **Save/Load System**

   - Binary file format
   - Preserve road layout and zone placement
5. **Public Transportation**

   - Bus stops/routes to reduce traffic demand
   - Basic public transport AI
6. **Advanced Traffic AI**

   - Pathfinding algorithms (A* or similar)
   - Dynamic route calculation based on traffic conditions
   - Lane changing and overtaking behavior
   - Congestion avoidance and rerouting
   - Emergency vehicle priority lanes

## Technical Architecture

### Core Data Structures

```cpp
// Pseudo-code structure examples

class Tile {
    int x, y;
    TileType type; // Empty, Road, Zone, etc.
    Node* node;    // If road/intersection
};

class Node {
    NodeType type; // Straight, Turn, T-Junction, 4-Way
    Direction connections[4]; // N, S, E, W
    vector<Lane> lanes;
    TrafficControl control; // None, Stop, Yield, Light
    // Note: Intersections only exist where roads explicitly connect
    // via start/end points, not through adjacency
};

class Road {
    vector<Node*> nodes;
    RoadType type;
    int laneCount;
    float speedLimit;
};

class Vehicle {
    Vector2 position;
    float speed;
    Node* currentNode;
    Node* targetNode;
    Lane* currentLane;
};
```

### System Architecture

```
Game/
├── Core/
│   ├── GameLoop.cpp
│   ├── InputHandler.cpp
│   └── SaveSystem.cpp
├── Roads/
│   ├── RoadManager.cpp
│   ├── RoadPlacement.cpp
│   ├── Node.cpp
│   └── Intersection.cpp
├── Traffic/
│   ├── TrafficManager.cpp
│   ├── Vehicle.cpp
│   ├── TrafficAI.cpp
│   └── SpawnPoint.cpp
├── UI/
│   ├── UIManager.cpp
│   ├── RoadToolbar.cpp
│   └── HUD.cpp
├── Rendering/
│   ├── MapRenderer.cpp
│   ├── CameraController.cpp
│   └── GridRenderer.cpp
└── Models/
    └── (Textures and 3D models if needed)
```

## Implementation Stages

### Stage 1: Foundation (Warm-up)

**Goal**: Basic map with road placement

1. Set up Raylib project with game loop
2. Implement perspective camera with pan/zoom
3. Create grid system with visual representation
4. Implement click-and-drag road placement (horizontal/vertical only)
5. Basic road rendering as colored tiles
6. Intersection detection when:
   - Player starts drawing from an existing road
   - Player ends drawing on an existing road
   - No automatic connection for adjacent roads

**Completion Criteria**: Player can place roads and see intersections form when starting from or ending on existing roads

### Stage 2: Traffic Introduction (Developing)

**Goal**: Moving vehicles on player-created roads

1. Create Node-based road network from placed roads
2. Implement traffic spawn points
3. Basic vehicle AI: follow road lanes
4. Yield-to-right intersection behavior
5. Variable vehicle speeds
6. Simple vehicle rendering (colored boxes)

**Completion Criteria**: Vehicles spawn and navigate the road network

### Stage 3: Advanced Intersections (Crossings)

**Goal**: Traffic control and improved AI

1. Intersection configuration UI
2. Traffic light system with timing
3. Stop sign and yield sign behavior
4. Vehicle AI responds to traffic controls
5. Turn lane support
6. Consider adding road elevation

**Completion Criteria**: Players can configure intersections and see traffic respond accordingly

### Stage 4: Zone System (Zones)

**Goal**: Purpose-driven traffic generation

1. Zone placement tools
2. Zone types with visual differentiation
3. Traffic demand based on zone combinations
4. Different vehicle types (cars, trucks, buses)
5. Zone-based spawn point generation

**Completion Criteria**: Traffic patterns emerge from zone placement

### Stage 5: Polish & Extras

**Goal**: Complete game experience

1. Save/load functionality
2. Public transportation system
3. Traffic flow visualization
4. Performance optimization
5. Additional road types
6. Basic sound effects

## AI System Design

### Traffic AI Phases

1. **Phase 1 - Basic Following**

   - Vehicles follow predetermined lanes
   - Fixed speed with simple deceleration
   - No collision detection (rely on spawn timing)
2. **Phase 2 - Intersection Logic**

   - Detect approaching intersections
   - Implement yield-to-right
   - Basic stop/go for traffic controls
3. **Phase 3 - Pathfinding**

   - Implement A* or similar for route planning
   - Dynamic rerouting for congestion
   - Lane changing logic

### Traffic Generation

- Spawn points generate vehicles based on:
  - Zone demand matrices
  - Time of day (if implemented)
  - Road capacity
- Vehicles despawn at map edges or designated points

## Development Guidelines

### Code Standards

- Use descriptive variable names
- Comment complex algorithms
- Keep functions focused and small
- Implement systems with expansion in mind

### Architecture Principles

1. **Modularity**: Each system should be independent
2. **Data-Driven**: Use configuration files where possible
3. **Observer Pattern**: For UI updates and system communication
4. **State Machines**: For AI and game states

### Performance Considerations

- Limit active vehicles based on performance
- Use spatial partitioning for collision detection
- LOD system for distant vehicles (if needed)
- Efficient pathfinding with caching

## Collaboration Rules

### When Implementing Features

1. Start with the simplest working version
2. Test each component independently
3. Add complexity incrementally
4. Always consider future expansions

### Problem-Solving Approach

1. Break complex problems into smaller tasks
2. Implement placeholder systems first
3. Optimize only when necessary
4. Prioritize functionality over visuals

### Communication Guidelines

- Explain technical decisions clearly
- Provide code examples when helpful
- Suggest alternatives for complex implementations
- Flag potential performance issues early

## Success Metrics

### MVP Success

- Roads can be placed and form a network
- Vehicles spawn and navigate successfully
- Basic traffic patterns emerge
- System remains performant with 100+ vehicles

### Extended Success

- Complex intersections function correctly
- Zone-based traffic feels realistic
- Save/load works reliably
- Game provides engaging sandbox experience

## Future Expansion Possibilities

- Building placement and growth
- Economy system
- Citizen needs and happiness
- Services (fire, police, healthcare)
- Terrain modification
- Weather effects
- Day/night cycle
- Multiplayer support

---

This document serves as the foundational blueprint for the city builder project. All development decisions should align with these specifications while maintaining flexibility for iterative improvements.
