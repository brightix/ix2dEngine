# Ix2dEngine Doc
## 一.引擎结构
object->Actor->Pawn
             ->GameModeBase

## 二. 核心模块
### 1. 主模块
#### 🎮 Actor 模块
- 描述：场景中一切元素的基类
- **常用接口**：
    - [`Tick()`](./)
    - [`SetPosition()`](./)

    <details>
    <summary>查看更多</summary>
  
    - [`SetPosition()`](./)

    </details>

### 🧍 Pawn 模块
- 描述：可被玩家或 AI 控制的角色

### 🎲 GameModeBase
- 描述：游戏规则与流程控制类

---
### 2. 子系统

#### (1) EnhancedInputSubSystem
- **描述**：提供玩家输入事件
- **接口**：

```cpp
void AddInputEventBool(
    SDL_Scancode scancode, 
    std::function<void(EnhancedInputParam<bool>)> func
);
```
**参数说明**:

- `scancode`：_描述按键的枚举值_

- `func`：_事件回调函数_
- 使用方法：
内联调用`func`。



## 3. 类关系图
（可以插入 UML 图或者用 Mermaid）
```mermaid
classDiagram
    Object <|-- Actor
    Actor <|-- Pawn
    Actor <|-- GameModeBase