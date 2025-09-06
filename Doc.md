# Ix2dEngine Doc
## 1.引擎结构
object->Actor->Pawn
             ->GameModeBase

## 2. 核心模块
### 🎮 Actor 模块
- 描述：场景中一切元素的基类
- 常用接口：
    - `Tick()`
    - `SetPosition()`

### 🧍 Pawn 模块
- 描述：可被玩家或 AI 控制的角色

### 🎲 GameModeBase
- 描述：游戏规则与流程控制类

---

## 3. 类关系图
（可以插入 UML 图或者用 Mermaid）
```mermaid
classDiagram
    Object <|-- Actor
    Actor <|-- Pawn
    Actor <|-- GameModeBase