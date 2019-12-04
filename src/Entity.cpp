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

#include "Entity.h"

#include "Component.h"

namespace NerdThings::Ngine {
    void Entity::_doDestroy() {
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

    Entity::Entity(const Vector2 position_, int depth_, bool canCull_)
            : m_canCull(canCull_), m_depth(depth_), m_position(position_),
              EntityContainer(EntityContainer::ENTITY) {}

    // Destructor

    Entity::~Entity() {
        // Fire event
        OnDestroy();

        // Destroy everything (being thorough).
        _doDestroy();

        // Remove from parent (if not already)
        if (m_parentEntity != nullptr || m_parentScene != nullptr)
            GetContainer()->RemoveChild(this);
    }

    bool Entity::CheckForCulling(Rectangle cullArea_) {
        return !cullArea_.Contains(GetPosition());
    }

    void Entity::Destroy() {
        // Remove from our parent
        GetContainer()->RemoveChild(this);
    }

    void Entity::Draw() {
        // Trigger draw
        OnDraw();

        // Debug draw
        if (m_physicsBody != nullptr) {
            if (m_physicsBody->GetWorld()->DebugDraw) {
                m_physicsBody->DebugDraw();
            }
        }
    }

    bool Entity::GetCanCull() {
        return m_canCull;
    }

    std::vector<Component *> Entity::GetComponents() {
        std::vector<Component *> vec;

        for (auto &_Component : m_components) {
            vec.push_back(_Component.second.get());
        }

        return vec;
    }

    int Entity::GetDepth() const {
        return m_depth;
    }

    EntityContainer *Entity::GetContainer() const {
        if (m_parentEntity != nullptr)
            return static_cast<EntityContainer *>(m_parentEntity);
        else if (m_parentScene != nullptr)
            return static_cast<EntityContainer *>(m_parentScene);
        return nullptr;
    }

    Scene *Entity::GetScene() const {
        return m_parentScene;
    }

    Game *Entity::GetGame() const {
        return m_parentScene->GetGame();
    }

    bool Entity::GetDoPersistentUpdates() {
        return m_persistentUpdates;
    }

    Vector2 Entity::GetPosition() const {
        if (m_physicsBody == nullptr) {
            return m_position;
        } else {
            return m_physicsBody->GetPosition();
        }
    }

    Physics::PhysicsBody *Entity::GetPhysicsBody() const {
        return m_physicsBody;
    }

    Physics::PhysicsWorld *Entity::GetPhysicsWorld() const {
        return m_physicsBody->GetWorld();
    }

    float Entity::GetRotation() const {
        if (m_physicsBody == nullptr) {
            return m_rotation;
        } else {
            return m_physicsBody->GetRotation();
        }
    }

    bool Entity::HasComponent(const std::string &name_) {
        return m_components.find(name_) != m_components.end();
    }

    bool Entity::IsPhysicsEntity() {
        return m_physicsBody != nullptr;
    }

    void Entity::MoveBy(const Vector2 moveBy_) {
        if (m_physicsBody == nullptr) {
            m_position += moveBy_;
        } else {
            auto p = m_physicsBody->GetPosition();
            m_physicsBody->SetPosition(p + moveBy_);
        }

        // Fire event
        OnTransformChanged({GetPosition(), GetRotation()});
    }

    bool Entity::RemoveComponent(const std::string &name_) {
        const auto comp = GetComponent<Component>(name_);

        if (comp != nullptr) {
            // Remove component from map
            m_components.erase(name_);

            return true;
        }

        // We don't have this component
        return false;
    }

    void Entity::SetCanCull(bool canCull_) {
        m_canCull = canCull_;
    }

    void Entity::SetDepth(int depth_) {
        m_parentScene->_updateEntityDepth(depth_, this);
        m_depth = depth_;
    }

    void Entity::SetDoPersistentUpdates(bool persistentUpdates_) {
        if (m_onUpdateRef.IsAttached())
            throw std::runtime_error("This property cannot be changed once update has been subscribed.");
        m_persistentUpdates = persistentUpdates_;
    }

    void Entity::SetPosition(const Vector2 position_) {
        if (m_physicsBody == nullptr) {
            m_position = position_;
        } else {
            m_physicsBody->SetPosition(position_);
        }

        // Fire event
        OnTransformChanged({GetPosition(), GetRotation()});
    }

    void Entity::SetRotation(float rotation_) {
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

    void Entity::SetPhysicsBody(Physics::PhysicsBody *body_) {
        auto physicsWorld = GetScene()->GetPhysicsWorld();
        if (physicsWorld == nullptr) throw std::runtime_error("Cannot make a physics entity in non-physics scene.");
        if (m_physicsBody != nullptr) m_physicsBody->Destroy(); // Destroy current fixture;
        m_physicsBody = body_;
    }

    bool Entity::SubscribeToUpdate() {
        if (m_parentScene != nullptr) {
            if (!m_onUpdateRef.IsAttached()) {
                if (m_persistentUpdates)
                    m_onUpdateRef = m_parentScene->OnPersistentUpdate += new ClassMethodEventHandler<Entity>(this,
                                                                                                             &Entity::Update);
                else
                    m_onUpdateRef = m_parentScene->OnUpdate += new ClassMethodEventHandler<Entity>(this,
                                                                                                   &Entity::Update);
                return true;
            } else {
                // We still have an event, soooo...
                return true;
            }
        }
        return false;
    }

    void Entity::UnsubscribeFromUpdate() {
        m_onUpdateRef.Detach();
    }

    void Entity::Update() {
        // Trigger update
        OnUpdate();

        if (m_physicsBody != nullptr) {
            // As we are a physics entity, fire this each frame
            OnTransformChanged({GetPosition(), GetRotation()});
        }
    }
}
