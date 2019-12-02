/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "BaseEntity.h"

#include "Component.h"

namespace NerdThings::Ngine {
    void BaseEntity::_doDestroy() {
        // Destroy physics body
        if (m_physicsBody != nullptr) m_physicsBody->Destroy();

        // Detach all events
        OnDraw.Clear();
        OnTransformChanged.Clear();
        OnUpdate.Clear();

        // Detach components
        m_components.clear();

        // Unsubscribe from update
        UnsubscribeFromUpdate();

        // Make our parents are null
        m_parentEntity = nullptr;
        m_parentScene = nullptr;
    }

    BaseEntity::BaseEntity(Scene *parentScene_, const Vector2 position_, int depth_, bool canCull_)
            : m_canCull(canCull_), m_depth(depth_), m_parentScene(parentScene_), m_position(position_),
              EntityContainer(EntityContainer::ENTITY) {
        if (parentScene_ == nullptr)
            throw std::runtime_error("Cannot give an entity a null parent scene.");

        // Set initial depth
        m_parentScene->InternalSetEntityDepth(m_depth, this);
    }

    // Destructor

    BaseEntity::~BaseEntity() {
        // Destroy everything (being thorough).
        _doDestroy();

        // Remove from parent (if not already)
        if (m_parentEntity != nullptr || m_parentScene != nullptr)
            GetContainer()->RemoveChild(this);
    }

    bool BaseEntity::CheckForCulling(Rectangle cullArea_) {
        return !cullArea_.Contains(GetPosition());
    }

    void BaseEntity::Destroy() {
        // Remove from our parent
        GetContainer()->RemoveChild(this);
    }

    void BaseEntity::Draw() {
        // Trigger draw
        OnDraw();

        // Debug draw
        if (m_physicsBody != nullptr) {
            if (m_physicsBody->GetWorld()->DebugDraw) {
                m_physicsBody->DebugDraw();
            }
        }
    }

    bool BaseEntity::GetCanCull() {
        return m_canCull;
    }

    std::vector<Component *> BaseEntity::GetComponents() {
        std::vector<Component *> vec;

        for (auto &_Component : m_components) {
            vec.push_back(_Component.second.get());
        }

        return vec;
    }

    int BaseEntity::GetDepth() const {
        return m_depth;
    }

    EntityContainer *BaseEntity::GetContainer() const {
        if (m_parentEntity != nullptr)
            return static_cast<EntityContainer *>(m_parentEntity);
        else if (m_parentScene != nullptr)
            return static_cast<EntityContainer *>(m_parentScene);
        return nullptr;
    }

    Scene *BaseEntity::GetScene() const {
        return m_parentScene;
    }

    Game *BaseEntity::GetGame() const {
        return m_parentScene->GetGame();
    }

    bool BaseEntity::GetDoPersistentUpdates() {
        return m_persistentUpdates;
    }

    Vector2 BaseEntity::GetPosition() const {
        if (m_physicsBody == nullptr) {
            return m_position;
        } else {
            return m_physicsBody->GetPosition();
        }
    }

    Physics::PhysicsBody *BaseEntity::GetPhysicsBody() const {
        return m_physicsBody;
    }

    Physics::PhysicsWorld *BaseEntity::GetPhysicsWorld() const {
        return m_physicsBody->GetWorld();
    }

    float BaseEntity::GetRotation() const {
        if (m_physicsBody == nullptr) {
            return m_rotation;
        } else {
            return m_physicsBody->GetRotation();
        }
    }

    bool BaseEntity::HasComponent(const std::string &name_) {
        return m_components.find(name_) != m_components.end();
    }

    bool BaseEntity::IsPhysicsEntity() {
        return m_physicsBody != nullptr;
    }

    void BaseEntity::MoveBy(const Vector2 moveBy_) {
        if (m_physicsBody == nullptr) {
            m_position += moveBy_;
        } else {
            auto p = m_physicsBody->GetPosition();
            m_physicsBody->SetPosition(p + moveBy_);
        }

        // Fire event
        OnTransformChanged({GetPosition(), GetRotation()});
    }

    bool BaseEntity::RemoveComponent(const std::string &name_) {
        const auto comp = GetComponent<Component>(name_);

        if (comp != nullptr) {
            // Remove component from map
            m_components.erase(name_);

            return true;
        }

        // We don't have this component
        return false;
    }

    void BaseEntity::SetCanCull(bool canCull_) {
        m_canCull = canCull_;
    }

    void BaseEntity::SetDepth(int depth_) {
        m_parentScene->InternalUpdateEntityDepth(m_depth, depth_, this);
        m_depth = depth_;
    }

    void BaseEntity::SetDoPersistentUpdates(bool persistentUpdates_) {
        if (m_onUpdateRef.IsAttached())
            throw std::runtime_error("This property cannot be changed once update has been subscribed.");
        m_persistentUpdates = persistentUpdates_;
    }

    void BaseEntity::SetPosition(const Vector2 position_) {
        if (m_physicsBody == nullptr) {
            m_position = position_;
        } else {
            m_physicsBody->SetPosition(position_);
        }

        // Fire event
        OnTransformChanged({GetPosition(), GetRotation()});
    }

    void BaseEntity::SetRotation(float rotation_) {
        if (m_physicsBody == nullptr) {
            // Set our rotation
            m_rotation = rotation_;
        } else {
            // Set the body rotation
            m_physicsBody->SetRotation(rotation_);
        }

        // Fire event
        OnTransformChanged({GetPosition(), GetRotation()});
    }

    void BaseEntity::SetPhysicsBody(Physics::PhysicsBody *body_) {
        auto physicsWorld = GetScene()->GetPhysicsWorld();
        if (physicsWorld == nullptr) throw std::runtime_error("Cannot make a physics entity in non-physics scene.");
        if (m_physicsBody != nullptr) m_physicsBody->Destroy(); // Destroy current fixture;
        m_physicsBody = body_;
    }

    bool BaseEntity::SubscribeToUpdate() {
        if (m_parentScene != nullptr) {
            if (!m_onUpdateRef.IsAttached()) {
                if (m_persistentUpdates)
                    m_onUpdateRef = m_parentScene->OnPersistentUpdate += new ClassMethodEventHandler<BaseEntity>(this,
                                                                                                                 &BaseEntity::Update);
                else
                    m_onUpdateRef = m_parentScene->OnUpdate += new ClassMethodEventHandler<BaseEntity>(this,
                                                                                                       &BaseEntity::Update);
                return true;
            } else {
                // We still have an event, soooo...
                return true;
            }
        }
        return false;
    }

    void BaseEntity::UnsubscribeFromUpdate() {
        m_onUpdateRef.Detach();
    }

    void BaseEntity::Update() {
        // Trigger update
        OnUpdate();

        if (m_physicsBody != nullptr) {
            // As we are a physics entity, fire this each frame
            OnTransformChanged({GetPosition(), GetRotation()});
        }
    }
}
