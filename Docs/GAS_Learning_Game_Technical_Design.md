# GAS 学习游戏技术设计文档

项目名：GASDemo  
引擎版本：Unreal Engine 5.4  
游戏类型：单机、俯视角、能力构筑、生存解谜  
核心目标：用尽可能少的美术资产，完整学习并实践 Gameplay Ability System，包括 Attribute、Gameplay Effect、Gameplay Ability、Gameplay Tag、Ability Task、Gameplay Cue、输入绑定、UI 监听和简单 AI 交互。

---

## 1. 项目愿景

本项目制作一个小型但机制完整的 GAS 学习游戏：**《频率回路 Frequency Circuit》**。

玩家控制一个没有复杂角色模型的几何体核心，在一个由方块、柱体、触发区和发光材质构成的训练场中生存。场地中不断出现“干扰节点”，玩家需要切换能力频率、吸收能量、过载敌人、修复核心，并在有限资源下完成回路校准。

游戏不依赖美术资产，优先使用：

- UE 基础几何体：Cube、Sphere、Cylinder、Plane。
- 纯色或发光材质。
- Niagara 简单粒子，后续可选。
- UMG 基础 UI。
- 音效可暂不实现，或使用引擎自带提示音替代。

本项目不是做大型游戏，而是做一个可以反复扩展、调试、观察 GAS 行为的学习沙盒。

---

## 2. 核心玩法

### 2.1 一句话玩法

玩家在俯视角 arena 中移动，用不同“频率能力”处理敌人和节点，通过积累 Sync 值完成校准，同时管理 Health、Energy、Heat、Shield 四类属性。

### 2.2 玩家目标

每局 3 到 5 分钟。玩家需要在时间或波次压力下，把场地中的 3 个“频率塔”校准到满值。

校准方式：

1. 靠近频率塔。
2. 持续按住 Interact。
3. 消耗 Energy，增加 Sync。
4. 敌人或干扰场会打断校准。
5. 三座塔全部完成后胜利。

### 2.3 失败条件

- Health 降为 0。
- 可选：总时间超过限制。

### 2.4 创意点：频率相性

所有敌人、节点和玩家能力都带有一个频率标签：

- Red：破坏频率，偏攻击。
- Blue：稳定频率，偏护盾和减速。
- Green：恢复频率，偏治疗和净化。

玩家可以切换当前频率。能力效果会根据玩家当前频率和目标频率产生变化。

示例：

- Red Pulse 命中 Red 敌人：普通伤害。
- Red Pulse 命中 Blue 敌人：伤害降低，但附加 Heat。
- Red Pulse 命中 Green 敌人：伤害提高，但反噬一点 Energy。
- Blue Field 对 Red 敌人：强减速。
- Green Surge 对自身：恢复 Health，同时移除 Burning。

这让 Gameplay Tag 不只是分类，而是驱动实际玩法逻辑。

---

## 3. 学习目标

本项目后续所有实现都围绕下列 GAS 学习目标展开。

### 3.1 必须覆盖

- AbilitySystemComponent 初始化。
- AttributeSet 定义、复制准备和 Clamp。
- GameplayEffect 的 Instant、Duration、Infinite 三种类型。
- GameplayAbility 激活、取消、冷却、消耗。
- GameplayTag 阻挡、状态、频率、输入映射。
- GameplayCue 用于轻量视觉反馈。
- Enhanced Input 与 Ability Input 绑定。
- Attribute 变化监听并驱动 UI。
- GameplayEffectContext 或 SetByCaller 用于传递动态伤害。
- 简单 AI 对玩家施加 GameplayEffect。

### 3.2 可选扩展

- AbilityTask 自定义。
- GameplayEffectExecutionCalculation 计算伤害。
- Modular Gameplay Effects。
- SaveGame 保存通关记录。
- DataAsset 驱动能力配置。
- 多角色或能力解锁。

---

## 4. 技术边界

### 4.1 项目现状

当前项目是一个基础 UE 5.4 C++ 项目：

- 模块名：`GASDemo`
- 当前依赖：`Core`、`CoreUObject`、`Engine`、`InputCore`、`EnhancedInput`
- 当前地图：`/Engine/Maps/Templates/OpenWorld`
- 当前没有 GAS 依赖和游戏类。

### 4.2 后续必须添加的模块依赖

在 `Source/GASDemo/GASDemo.Build.cs` 中添加：

```csharp
PublicDependencyModuleNames.AddRange(new string[]
{
    "Core",
    "CoreUObject",
    "Engine",
    "InputCore",
    "EnhancedInput",
    "GameplayAbilities",
    "GameplayTags",
    "GameplayTasks",
    "UMG",
    "AIModule"
});
```

### 4.3 后续必须启用的插件

在 `GASDemo.uproject` 中启用：

- `GameplayAbilities`

Enhanced Input 已经可用。

### 4.4 单机原则

项目先按单机实现，但 C++ 结构预留网络友好习惯：

- Attribute 使用标准 GAS 宏。
- AbilitySystemComponent 放在 PlayerState 还是 Character 需要明确。
- 本项目为学习简化，先放在 Character/Pawn 上。
- 暂不实现网络复制，但命名和 API 使用尽量贴近 GAS 标准写法。

---

## 5. 游戏结构

### 5.1 推荐目录

```text
Source/GASDemo/
  AbilitySystem/
    GDAbilitySystemComponent.h/.cpp
    GDAttributeSet.h/.cpp
    GDGameplayAbility.h/.cpp
    GDGameplayTags.h/.cpp
  Characters/
    GDPlayerPawn.h/.cpp
    GDEnemyPawn.h/.cpp
  Controllers/
    GDPlayerController.h/.cpp
    GDEnemyAIController.h/.cpp
  Game/
    GDGameMode.h/.cpp
    GDGameState.h/.cpp
  Interaction/
    GDFrequencyTower.h/.cpp
    GDEnergyOrb.h/.cpp
    GDHazardVolume.h/.cpp
  UI/
    GDHUDWidget.h/.cpp
  Data/
    GDAbilitySet.h/.cpp
```

```text
Content/GASDemo/
  Blueprints/
    Characters/
    Game/
    Interaction/
  Abilities/
    GA_*
    GE_*
    GC_*
  Input/
    IA_Move
    IA_Primary
    IA_Secondary
    IA_ShiftFrequency
    IA_Interact
    IMC_Gameplay
  Maps/
    M_LearningArena
  Materials/
    M_Frequency_Red
    M_Frequency_Blue
    M_Frequency_Green
    M_Player
    M_Enemy
  UI/
    WBP_HUD
```

说明：C++ 定义核心行为，蓝图负责配置具体 Ability、GameplayEffect、材质和 UI 外观。

---

## 6. 角色与控制

### 6.1 玩家 Pawn

类名：`AGDPlayerPawn`

推荐继承：`APawn`，自己组合组件，避免初期引入 Character Movement 的复杂性。

组件：

- `USceneComponent* Root`
- `UStaticMeshComponent* BodyMesh`
- `UFloatingPawnMovement* MovementComponent`
- `UCameraComponent* Camera`
- `USpringArmComponent* SpringArm`
- `UGDAbilitySystemComponent* AbilitySystemComponent`
- `UGDAttributeSet* AttributeSet`

表现：

- BodyMesh 使用基础 Sphere 或 Cube。
- 频率改变时切换材质颜色。
- 俯视角相机固定 55 到 65 度。

输入：

- WASD：移动。
- Left Mouse / J：Primary Pulse。
- Right Mouse / K：Secondary Field。
- Space：Dash。
- Q：Shift Frequency。
- E：Interact / Calibrate。
- R：Cleanse / Recover。

### 6.2 敌人 Pawn

类名：`AGDEnemyPawn`

推荐继承：`APawn`。

组件：

- `UStaticMeshComponent* BodyMesh`
- `UFloatingPawnMovement* MovementComponent`
- `UGDAbilitySystemComponent* AbilitySystemComponent`
- `UGDAttributeSet* AttributeSet`

敌人类型：

1. **Chaser 干扰体**
   - 追踪玩家。
   - 触碰造成伤害。
   - 学习点：Overlap 后 Apply GameplayEffect。

2. **Emitter 发射体**
   - 固定或慢速移动。
   - 周期发射干扰波。
   - 学习点：AI 激活 Ability。

3. **Leech 吸能体**
   - 靠近后吸取 Energy。
   - 学习点：Duration GameplayEffect。

---

## 7. Attribute 设计

类名：`UGDAttributeSet`

### 7.1 玩家和敌人共用属性

| 属性 | 类型 | 说明 |
|---|---|---|
| Health | float | 当前生命 |
| MaxHealth | float | 最大生命 |
| Energy | float | 当前能量 |
| MaxEnergy | float | 最大能量 |
| Shield | float | 护盾值，优先吸收伤害 |
| MaxShield | float | 最大护盾 |
| Heat | float | 热量，过高会进入 Overheated |
| MaxHeat | float | 最大热量 |
| MoveSpeed | float | 移动速度 |
| Sync | float | 校准进度 |
| MaxSync | float | 最大校准进度 |
| Damage | float | Meta Attribute，用于伤害结算 |

### 7.2 Clamp 规则

- Health：`0 到 MaxHealth`
- Energy：`0 到 MaxEnergy`
- Shield：`0 到 MaxShield`
- Heat：`0 到 MaxHeat`
- Sync：`0 到 MaxSync`
- MoveSpeed：不低于 `100`

### 7.3 Damage 处理规则

`Damage` 作为 Meta Attribute，不直接显示在 UI。

伤害进入 `PostGameplayEffectExecute`：

1. 如果 Damage <= 0，忽略。
2. Shield 先抵扣。
3. 剩余伤害扣 Health。
4. Damage 清零。
5. Health 为 0 时添加 `State.Dead` Tag 或触发死亡事件。

### 7.4 初始值

玩家：

- Health：100
- MaxHealth：100
- Energy：100
- MaxEnergy：100
- Shield：25
- MaxShield：50
- Heat：0
- MaxHeat：100
- MoveSpeed：600
- Sync：0
- MaxSync：100

Chaser：

- Health：35
- MaxHealth：35
- Energy：0
- Shield：0
- Heat：0
- MoveSpeed：420

Emitter：

- Health：60
- MaxHealth：60
- Shield：15
- MoveSpeed：150

Leech：

- Health：45
- MaxHealth：45
- MoveSpeed：360

---

## 8. Gameplay Tag 体系

Tag 文件后续放入 `Config/DefaultGameplayTags.ini`，或通过 Project Settings 管理。

### 8.1 输入 Tag

```text
Input.Primary
Input.Secondary
Input.Dash
Input.ShiftFrequency
Input.Interact
Input.Recover
```

### 8.2 Ability Tag

```text
Ability.PrimaryPulse
Ability.StabilityField
Ability.Dash
Ability.ShiftFrequency
Ability.Calibrate
Ability.Recover
Ability.Enemy.TouchDamage
Ability.Enemy.InterferenceShot
Ability.Enemy.EnergyLeech
```

### 8.3 状态 Tag

```text
State.Dead
State.Stunned
State.Overheated
State.Shielded
State.Calibrating
State.EnergyLeeching
State.Invulnerable
State.Burning
State.Slowed
```

### 8.4 频率 Tag

```text
Frequency.Red
Frequency.Blue
Frequency.Green
```

### 8.5 Cue Tag

```text
GameplayCue.Pulse.Red
GameplayCue.Pulse.Blue
GameplayCue.Pulse.Green
GameplayCue.Hit
GameplayCue.ShieldBreak
GameplayCue.Calibrate
GameplayCue.Overheated
GameplayCue.Recover
```

### 8.6 Effect Tag

```text
Effect.Damage
Effect.Heal
Effect.Cost
Effect.Cooldown
Effect.Buff
Effect.Debuff
```

---

## 9. Gameplay Ability 设计

所有 Ability 蓝图继承自 C++ 基类 `UGDGameplayAbility`。

### 9.1 C++ Ability 基类

类名：`UGDGameplayAbility`

职责：

- 保存 `InputTag`。
- 提供 `GetGDAbilitySystemComponentFromActorInfo()`。
- 提供 Commit 检查辅助函数。
- 提供频率读取函数。
- 统一设置 Instancing Policy：`InstancedPerActor`。

### 9.2 GA_PrimaryPulse

类型：主动攻击。

输入：`Input.Primary`

效果：

- 从玩家当前位置向鼠标方向或朝向发射一个短距离球形 Trace。
- 命中敌人后应用 `GE_Damage_PrimaryPulse`。
- 根据当前频率添加 Cue。

GAS 学习点：

- Ability 激活。
- Commit Cost。
- Commit Cooldown。
- SetByCaller 传递伤害。
- GameplayCue 反馈。

参数：

- Cost：Energy -10。
- Cooldown：0.35 秒。
- BaseDamage：20。
- Range：900。
- Radius：80。

频率加成：

- Red：Damage +8，Heat +8。
- Blue：Damage -4，添加 Slowed。
- Green：Damage -2，命中后自身 Heal +5。

### 9.3 GA_StabilityField

类型：范围控制。

输入：`Input.Secondary`

效果：

- 在玩家周围生成 3 秒稳定场。
- 场内敌人获得 Slow。
- 玩家获得 Shield Regen。

GAS 学习点：

- Duration Effect。
- Periodic Effect。
- Ability 生命周期和取消。
- GameplayCue Loop。

参数：

- Cost：Energy -25。
- Cooldown：6 秒。
- Radius：500。
- Duration：3 秒。

效果：

- `GE_Debuff_Slow_StabilityField`
- `GE_Buff_ShieldRegen_StabilityField`

### 9.4 GA_Dash

类型：位移。

输入：`Input.Dash`

效果：

- 朝移动方向快速冲刺。
- 0.25 秒内添加 Invulnerable。
- 冲刺结束添加少量 Heat。

GAS 学习点：

- AbilityTask 或 Timer。
- 临时状态 Tag。
- Cooldown。

参数：

- Cost：Energy -15。
- Cooldown：2.5 秒。
- Distance：550。
- Invulnerable Duration：0.25 秒。

### 9.5 GA_ShiftFrequency

类型：状态切换。

输入：`Input.ShiftFrequency`

效果：

- Red -> Blue -> Green -> Red 循环。
- 移除旧频率 Tag。
- 添加新频率 Tag。
- 更新玩家材质。

GAS 学习点：

- ASC Loose Gameplay Tag 管理。
- Tag 变化监听。
- UI 状态刷新。

参数：

- Cooldown：0.2 秒。
- 无 Cost。

### 9.6 GA_Calibrate

类型：交互。

输入：`Input.Interact`

效果：

- 玩家在塔附近按住 E 时激活。
- 持续消耗 Energy。
- 给目标塔增加 Sync。
- 被攻击、离开范围或 Energy 不足时取消。

GAS 学习点：

- 持续 Ability。
- Ability 取消条件。
- Periodic Cost。
- 与非角色 Actor 交互。

参数：

- 每秒 Energy -12。
- 每秒 Sync +18。
- 需要目标有 `Interactable.FrequencyTower`。

### 9.7 GA_Recover

类型：自我恢复。

输入：`Input.Recover`

效果：

- 消耗 Sync 或 Energy，恢复 Health。
- 移除 Burning / Slowed。
- 若 Heat 太高，则降低 Heat。

GAS 学习点：

- Remove Active Effects With Tags。
- 多个 GameplayEffect 串联。
- 资源交换。

参数：

- Cost：Energy -30。
- Cooldown：8 秒。
- Heal：25。
- Heat -20。

### 9.8 敌人 Ability

#### GA_Enemy_TouchDamage

- 触碰玩家时施加 Damage。
- Cooldown：1 秒。
- Damage：12。

#### GA_Enemy_InterferenceShot

- Emitter 周期释放远程干扰。
- 命中玩家施加 Damage + Burning。
- Cooldown：2.5 秒。
- Damage：10。

#### GA_Enemy_EnergyLeech

- Leech 靠近玩家后施加 Duration Effect。
- 每秒 Energy -8。
- 持续 4 秒。
- 玩家离开范围则移除。

---

## 10. Gameplay Effect 设计

### 10.1 初始化类

| 名称 | 类型 | 说明 |
|---|---|---|
| GE_Player_InitAttributes | Instant | 初始化玩家属性 |
| GE_Chaser_InitAttributes | Instant | 初始化 Chaser |
| GE_Emitter_InitAttributes | Instant | 初始化 Emitter |
| GE_Leech_InitAttributes | Instant | 初始化 Leech |

### 10.2 消耗类

| 名称 | 类型 | 修改 |
|---|---|---|
| GE_Cost_PrimaryPulse | Instant | Energy -10 |
| GE_Cost_StabilityField | Instant | Energy -25 |
| GE_Cost_Dash | Instant | Energy -15 |
| GE_Cost_Recover | Instant | Energy -30 |
| GE_Cost_Calibrate_Tick | Instant 或 Periodic | Energy -12/s |

### 10.3 冷却类

| 名称 | 类型 | Duration |
|---|---|---|
| GE_Cooldown_PrimaryPulse | Duration | 0.35 |
| GE_Cooldown_StabilityField | Duration | 6 |
| GE_Cooldown_Dash | Duration | 2.5 |
| GE_Cooldown_ShiftFrequency | Duration | 0.2 |
| GE_Cooldown_Recover | Duration | 8 |

每个冷却 GE 添加对应 Cooldown Tag：

```text
Cooldown.PrimaryPulse
Cooldown.StabilityField
Cooldown.Dash
Cooldown.ShiftFrequency
Cooldown.Recover
```

### 10.4 战斗类

| 名称 | 类型 | 修改 |
|---|---|---|
| GE_Damage_SetByCaller | Instant | Damage = SetByCaller.Data.Damage |
| GE_Heal_Recover | Instant | Health +25 |
| GE_Heat_Add | Instant | Heat + SetByCaller.Data.Heat |
| GE_Heat_Reduce | Instant | Heat -20 |
| GE_Shield_Add | Instant | Shield + SetByCaller.Data.Shield |
| GE_Buff_ShieldRegen | Duration Periodic | Shield +8/s |
| GE_Debuff_Slow | Duration | MoveSpeed * 0.5 |
| GE_Debuff_Burning | Duration Periodic | Damage +4/s |
| GE_Debuff_EnergyLeech | Duration Periodic | Energy -8/s |
| GE_State_Invulnerable | Duration | Granted Tag State.Invulnerable |
| GE_State_Overheated | Duration | Granted Tag State.Overheated |

### 10.5 Heat 规则

Heat 是学习状态系统的关键。

- 使用 Red 能力会增加 Heat。
- Heat >= MaxHeat 时进入 Overheated。
- Overheated 持续 4 秒。
- Overheated 期间：
  - 阻挡 `Ability.PrimaryPulse`
  - 阻挡 `Ability.Dash`
  - MoveSpeed 降低 25%
  - 每秒 Energy -5
- Overheated 结束后 Heat 归零或降到 30。

实现方式：

1. AttributeSet 中监听 Heat。
2. 当 Heat 达到 MaxHeat 时，ASC 应用 `GE_State_Overheated`。
3. Ability 的 Activation Blocked Tags 添加 `State.Overheated`。

---

## 11. 频率系统设计

### 11.1 当前频率的存储

使用 ASC Loose Gameplay Tag 表示当前频率：

- 默认：`Frequency.Red`
- 切换时确保三种频率只有一个存在。

### 11.2 C++ 辅助函数

在 `UGDAbilitySystemComponent` 中提供：

```cpp
FGameplayTag GetCurrentFrequencyTag() const;
void SetCurrentFrequencyTag(FGameplayTag NewFrequencyTag);
```

在 `UGDGameplayAbility` 中提供：

```cpp
FGameplayTag GetCurrentFrequency() const;
```

### 11.3 频率相性表

| 攻击方 | 目标 | 效果 |
|---|---|---|
| Red | Red | 正常 |
| Red | Blue | 伤害 -20%，目标 Heat +10 |
| Red | Green | 伤害 +25%，自身 Energy -5 |
| Blue | Red | Slow 强化 |
| Blue | Blue | Shield 效果增强 |
| Blue | Green | 伤害降低，清除目标 Burning |
| Green | Red | Heal 自身但低伤害 |
| Green | Blue | Shield 回复 |
| Green | Green | 恢复增强 |

后续第一版实现不需要覆盖全部组合，至少实现：

- Red：高伤害 + 自身 Heat。
- Blue：低伤害 + Slow。
- Green：低伤害 + 自身 Heal。

---

## 12. 输入系统

使用 Enhanced Input。

### 12.1 Input Actions

| 资源 | 类型 | 绑定 |
|---|---|---|
| IA_Move | Axis2D | WASD |
| IA_Primary | Bool | Left Mouse / J |
| IA_Secondary | Bool | Right Mouse / K |
| IA_Dash | Bool | Space |
| IA_ShiftFrequency | Bool | Q |
| IA_Interact | Bool | E |
| IA_Recover | Bool | R |

### 12.2 Ability 输入绑定策略

推荐使用 GameplayTag 作为 Ability 输入标识：

- Ability 有 `InputTag`。
- PlayerController 或 Pawn 在输入触发时调用 ASC：
  - `AbilityInputTagPressed(InputTag)`
  - `AbilityInputTagReleased(InputTag)`

`UGDAbilitySystemComponent` 需要维护：

```cpp
void AbilityInputTagPressed(const FGameplayTag& InputTag);
void AbilityInputTagReleased(const FGameplayTag& InputTag);
void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
```

第一版也可以直接在 Pawn 输入事件中调用 `TryActivateAbilitiesByTag`，但长期建议实现 Lyra 风格的输入队列，方便学习。

---

## 13. UI 设计

UI 类：`UGDHUDWidget` 或蓝图 `WBP_HUD`。

### 13.1 HUD 元素

- Health Bar
- Energy Bar
- Shield Bar
- Heat Bar
- Sync Progress
- 当前 Frequency 指示器
- Ability Cooldown 简单文本或进度条
- 当前目标提示
- 胜利/失败提示

### 13.2 UI 数据来源

HUD 监听 ASC Attribute 变化：

```cpp
GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(...)
```

Tag 监听：

```cpp
RegisterGameplayTagEvent(Tag, EGameplayTagEventType::NewOrRemoved)
```

### 13.3 视觉原则

不做复杂美术，使用颜色表达状态：

- Red：红/橙，攻击与 Heat。
- Blue：蓝/青，Shield 与 Slow。
- Green：绿，Heal 与 Cleanse。
- Heat 满时 UI 闪烁或变橙。
- Overheated 时玩家材质变白或深红。

---

## 14. 关卡设计

地图：`Content/GASDemo/Maps/M_LearningArena`

### 14.1 场景结构

- 一个 4000 x 4000 的平面。
- 四周用 Cube 做墙。
- 中央放玩家出生点。
- 三个 Frequency Tower 呈三角分布。
- 若干柱体作为障碍。
- 边缘生成敌人。

### 14.2 资产限制

只使用：

- Basic Shapes。
- 简单材质。
- Point Light / Rect Light。
- 可选 Niagara。

### 14.3 交互物

#### Frequency Tower

类名：`AGDFrequencyTower`

属性：

- CurrentSync
- MaxSync = 100
- RequiredFrequency
- bCompleted

行为：

- 玩家进入范围时成为可交互目标。
- 校准时增加 CurrentSync。
- 完成后改变材质并广播事件。

#### Energy Orb

类名：`AGDEnergyOrb`

行为：

- 玩家拾取后应用 `GE_EnergyRestore`。
- 可用 Sphere + 发光材质。

#### Hazard Volume

类名：`AGDHazardVolume`

行为：

- 玩家进入后周期受到 Damage 或 Heat。
- 学习点：体积触发 GameplayEffect。

---

## 15. 游戏流程

### 15.1 启动

1. GameMode 生成 Player Pawn。
2. Player Pawn 初始化 ASC。
3. 应用 `GE_Player_InitAttributes`。
4. 授予默认 Ability。
5. 设置默认频率 `Frequency.Red`。
6. 创建 HUD。

### 15.2 每局流程

1. 玩家进入 Arena。
2. 敌人 Spawner 每隔数秒生成敌人。
3. 玩家清理敌人并校准 Frequency Tower。
4. 三座塔完成后胜利。
5. Health 为 0 时失败。

### 15.3 波次节奏

第一版：

- 第 0 秒：2 个 Chaser。
- 第 30 秒：每 10 秒生成 2 个 Chaser。
- 第 60 秒：加入 Emitter。
- 第 90 秒：加入 Leech。

后续可调整为 DataTable。

---

## 16. C++ 类职责

### 16.1 UGDAbilitySystemComponent

职责：

- 输入 Tag 到 Ability 激活。
- 当前频率 Tag 设置。
- Ability 授予。
- Debug 打印当前 Tag / Attribute。

关键函数：

```cpp
void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
void AbilityInputTagPressed(const FGameplayTag& InputTag);
void AbilityInputTagReleased(const FGameplayTag& InputTag);
void SetCurrentFrequencyTag(FGameplayTag NewFrequencyTag);
FGameplayTag GetCurrentFrequencyTag() const;
```

### 16.2 UGDAttributeSet

职责：

- 声明 Attribute。
- Clamp。
- Damage 分流到 Shield 和 Health。
- 触发 Death 回调。
- 检测 Heat 上限。

### 16.3 UGDGameplayAbility

职责：

- 保存 AbilityTag 和 InputTag。
- 封装常用 ASC 访问。
- 提供 Commit 前日志。

### 16.4 AGDPlayerPawn

职责：

- 拥有 ASC。
- 初始化 Attribute 和 Ability。
- 处理移动输入。
- 把 Ability 输入转发给 ASC。
- 根据频率切换材质。
- 保存当前可交互目标。

### 16.5 AGDEnemyPawn

职责：

- 拥有 ASC。
- 初始化 Attribute 和 Ability。
- 存储 EnemyType 和 Frequency。
- 死亡时销毁或播放简单效果。

### 16.6 AGDFrequencyTower

职责：

- 提供交互范围。
- 接收校准进度。
- 完成后通知 GameMode/GameState。

### 16.7 AGDGameMode

职责：

- 指定默认 Pawn、Controller、HUD。
- 管理胜负。
- 生成敌人。

---

## 17. 蓝图职责

### 17.1 必须蓝图化的内容

- 具体 GameplayAbility：
  - `GA_PrimaryPulse`
  - `GA_StabilityField`
  - `GA_Dash`
  - `GA_ShiftFrequency`
  - `GA_Calibrate`
  - `GA_Recover`
- 具体 GameplayEffect：
  - 初始化、伤害、治疗、消耗、冷却、Buff、Debuff。
- HUD Widget。
- 材质和关卡摆放。

### 17.2 推荐 C++ 化的内容

- ASC 输入桥接。
- AttributeSet。
- Pawn 初始化。
- Tower 交互接口。
- 敌人基础行为。

原因：这些内容是 GAS 学习核心，C++ 代码能更清楚地观察生命周期和调试。

---

## 18. 调试与可视化

### 18.1 控制台命令

后续实现自定义 Exec 命令或 Debug Key：

- 打印玩家 Attribute。
- 打印玩家 Owned Tags。
- 给玩家恢复 Energy。
- 强制切换频率。
- 生成敌人。

### 18.2 屏幕 Debug

开发阶段允许使用：

```cpp
GEngine->AddOnScreenDebugMessage(...)
```

显示：

- Ability 激活失败原因。
- 当前频率。
- 当前 Heat。
- 目标受到的 Damage。

### 18.3 GAS Debugger

运行时使用：

```text
showdebug abilitysystem
```

检查：

- ASC 是否初始化。
- Attribute 是否正确。
- Active GameplayEffect 是否正确。
- Owned Tag 是否正确。

---

## 19. 实现阶段计划

### Phase 0：项目 GAS 基础配置

目标：项目能编译并具备 GAS 基础类。

任务：

1. 修改 `GASDemo.Build.cs`，加入 GAS 依赖。
2. 修改 `GASDemo.uproject`，启用 GameplayAbilities 插件。
3. 新建 `UGDAbilitySystemComponent`。
4. 新建 `UGDAttributeSet`。
5. 新建 `UGDGameplayAbility`。
6. 添加 Gameplay Tags 配置。

验收：

- 项目编译通过。
- C++ 能引用 `AbilitySystemComponent.h`。
- `showdebug abilitysystem` 不报错。

### Phase 1：玩家 Pawn 与基础属性

目标：场景中可控制玩家，HUD 能显示属性。

任务：

1. 新建 `AGDPlayerPawn`。
2. 添加 Mesh、Movement、Camera。
3. 初始化 ASC 和 AttributeSet。
4. 应用初始属性 GE。
5. 配置 Enhanced Input。
6. 创建基础 HUD。

验收：

- 玩家能移动。
- Health/Energy/Shield/Heat 显示正确。
- 初始频率为 Red。

### Phase 2：基础 Ability

目标：实现三个核心主动能力。

任务：

1. 实现 `GA_ShiftFrequency`。
2. 实现 `GA_PrimaryPulse`。
3. 实现 `GA_Dash`。
4. 实现消耗和冷却 GE。
5. 加入简单 Cue 或 Debug 视觉反馈。

验收：

- Q 能切换频率。
- Primary 能消耗 Energy 并进入冷却。
- Dash 能位移并短暂无敌。
- Energy 不足时 Ability 激活失败。

### Phase 3：敌人与伤害结算

目标：敌人可被攻击，敌人能伤害玩家。

任务：

1. 实现 `AGDEnemyPawn`。
2. 实现 Chaser AI。
3. 实现 TouchDamage。
4. 实现 Damage Meta Attribute 结算。
5. 实现敌人死亡。

验收：

- PrimaryPulse 能击杀敌人。
- 敌人接触玩家会扣 Shield/Health。
- Health 为 0 后玩家失败。

### Phase 4：频率塔与胜利流程

目标：玩家能校准目标并完成游戏。

任务：

1. 实现 `AGDFrequencyTower`。
2. 实现交互检测。
3. 实现 `GA_Calibrate`。
4. GameMode 统计完成数量。
5. HUD 显示 Sync。

验收：

- 玩家靠近塔能校准。
- 校准消耗 Energy。
- 三座塔完成后胜利。

### Phase 5：控制、恢复与状态

目标：让 GAS 状态系统更完整。

任务：

1. 实现 `GA_StabilityField`。
2. 实现 `GA_Recover`。
3. 实现 Burning、Slow、Overheated。
4. UI 显示状态。
5. 加入 Emitter 和 Leech。

验收：

- StabilityField 能减速敌人并恢复 Shield。
- Recover 能治疗和净化。
- Heat 满后进入 Overheated 并阻挡部分 Ability。

### Phase 6：打磨与学习辅助

目标：让项目适合复盘和继续扩展。

任务：

1. 增加 Debug 面板。
2. 增加能力说明 UI。
3. 整理蓝图命名。
4. 添加 README 操作说明。
5. 记录 GAS 学习笔记。

验收：

- 新用户能按 README 运行。
- 文档、代码、蓝图命名一致。
- 能清楚观察每种 GAS 机制。

---

## 20. 命名规范

### 20.1 C++ 前缀

- `GD` 表示 GASDemo。
- UObject：`UGD*`
- Actor：`AGD*`
- Struct：`FGD*`
- Enum：`EGD*`

### 20.2 蓝图前缀

- `BP_`：普通蓝图。
- `GA_`：Gameplay Ability。
- `GE_`：Gameplay Effect。
- `GC_`：Gameplay Cue。
- `WBP_`：Widget。
- `IA_`：Input Action。
- `IMC_`：Input Mapping Context。
- `M_`：Material。
- `MI_`：Material Instance。

### 20.3 Tag 命名

使用层级式名词：

```text
Ability.PrimaryPulse
State.Overheated
Frequency.Red
Input.Primary
GameplayCue.Pulse.Red
```

---

## 21. 最小可玩版本定义

最小可玩版本必须包含：

1. 一个可移动玩家。
2. Health、Energy、Shield、Heat。
3. 至少三个玩家 Ability：
   - PrimaryPulse
   - Dash
   - ShiftFrequency
4. 至少一种敌人：
   - Chaser
5. 至少一个可校准 Frequency Tower。
6. 胜利和失败条件。
7. 基础 HUD。
8. 不依赖外部美术资产。

---

## 22. 风险与决策

### 22.1 风险：蓝图资产无法纯文本直接生成

UE 蓝图资源是 `.uasset`，后续自动化实现时，C++ 类、配置和文档可以直接编辑，但蓝图资源通常需要在编辑器中创建。

应对：

- 尽量把核心逻辑放到 C++。
- 蓝图只做数据配置。
- 必要时创建 C++ GameplayEffect 子类替代部分 GE 蓝图。

### 22.2 风险：GAS 对初始化顺序敏感

ASC 必须正确调用：

```cpp
AbilitySystemComponent->InitAbilityActorInfo(OwnerActor, AvatarActor);
```

应对：

- 玩家 Pawn 在 `PossessedBy` 或 `BeginPlay` 中统一初始化。
- 单机项目先用 Pawn 自持 ASC，降低 PlayerState 初始化复杂度。

### 22.3 风险：能力输入绑定复杂

第一版可用简单方案：

```cpp
ASC->TryActivateAbilitiesByTag(...)
```

后续再升级为 InputTag 队列。

### 22.4 风险：低资产导致反馈弱

应对：

- 用材质颜色表达频率。
- 用 Scale、闪烁、发光表达状态。
- 用 Debug Text 辅助学习。
- 后续再补 Niagara。

---

## 23. 后续工作约定

后续所有实现以本文档为准。

如果实现中发现文档设计与 UE/GAS 实际机制冲突，处理顺序为：

1. 保持学习目标不变。
2. 优先选择 GAS 官方推荐实践。
3. 修改文档记录新决策。
4. 再修改代码。

每个阶段完成后需要更新：

- 本文档的阶段状态。
- README 的运行说明。
- 必要时添加学习笔记。

---

## 24. 当前阶段状态

| 阶段 | 状态 |
|---|---|
| Phase 0：项目 GAS 基础配置 | 已完成 |
| Phase 1：玩家 Pawn 与基础属性 | 进行中 |
| Phase 2：基础 Ability | 未开始 |
| Phase 3：敌人与伤害结算 | 未开始 |
| Phase 4：频率塔与胜利流程 | 未开始 |
| Phase 5：控制、恢复与状态 | 未开始 |
| Phase 6：打磨与学习辅助 | 未开始 |

当前进度说明：

- Phase 0 已完成 GAS 基础配置：项目依赖、GameplayAbilities 插件、基础 ASC、AttributeSet、GameplayAbility 基类和 Gameplay Tags 已建立。
- Phase 1 已完成玩家 Pawn、基础移动输入、ASC 初始化、属性初始化 GE 配置与应用。已在编辑器中通过 `ShowDebug AbilitySystem` 确认初始化属性数值能正常显示。
- Phase 1 已新增 `UGDHUDWidget` C++ HUD 数据基类，支持监听 Health、Energy、Shield、Heat、Sync 及 Max 值变化，并监听当前 Frequency Tag。`AGDPlayerPawn` 已支持创建 HUD Widget 并传入 ASC/AttributeSet。
- Phase 1 下一步继续在编辑器中创建 `WBP_HUD` 并继承 `UGDHUDWidget`，完成属性条和频率指示器的视觉绑定，然后补齐后续 Ability 输入入口。
