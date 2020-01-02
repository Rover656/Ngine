/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright (C) 2020 NerdThings.
*
*   LICENSE: GNU LGPLv3
*   View: In Ngine.hpp
*
**********************************************************************************************/

#include "Entity.hpp"

#include "Component.hpp"

namespace NerdThings::Ngine {
    void Entity::_addToScene(Scene *scene_) {
        if (m_parentScene != nullptr)
            throw std::runtime_error("Cannot add entity to more than one scene. Remove from current scene first.");

        // Set the parent scene
        m_parentScene = scene_;

        // TODO: Check scene is physics enabled too!!!
        // Create physics body if physics is enabled
        if (m_physicsEnabled) {
            // Create body
            m_physicsBody = new Physics::PhysicsBody(m_parentScene->GetPhysicsWorld());

            // Send initial position and rotation to body (TODO: body center system too!)
            m_physicsBody->SetTransform(m_transform);
        }

        // Add to scene
        m_parentScene->_addEntity(this);
    }

    void Entity::_removeFromScene() {
        // Unbind update event
        UnsubscribeFromUpdate();

        // Delete physics body
        if (m_physicsEnabled) {
            // Delete body
            delete m_physicsBody;
            m_physicsBody = nullptr;
        }

        // Remove from scene (and therefore unset entity parent too)
        m_parentEntity = nullptr;
        m_parentScene = nullptr;
    }

    void Entity::_doDestroy() {
        // Fire event
        OnDestroy();

        // Delete components
        for (const auto &c : m_components)
            delete c.second;
        m_components.clear();

        // Destroy physics body
        if (m_physicsBody != nullptr) m_physicsBody->Destroy();

        // Detach all events
        OnDraw.Clear();
        OnTransformChanged.Clear();
        OnUpdate.Clear();

        // Unsubscribe from update
        UnsubscribeFromUpdate();

        // Make our parents are null
        m_parentEntity = nullptr;
        m_parentScene = nullptr;
    }

    // TODO: Remove position and rotation from this constructor???
    Entity::Entity(const Vector2 position_, float rotation_, int depth_, bool canCull_, bool physicsEnabled_)
            : m_canCull(canCull_), m_depth(depth_), m_transform(position_, rotation_),
              m_physicsEnabled(physicsEnabled_),
              EntityContainer(EntityContainer::ENTITY) {}

    Entity::~Entity() {
        // Remove from parent (if not already)
        if (m_parentEntity != nullptr || m_parentScene != nullptr)
            GetContainer()->RemoveChild(this);

        // Destroy everything (being thorough).
        _doDestroy();
    }

    void Entity::Destroy() {
        if (GetContainer() != nullptr) {
            // We have a container, so we detach from it
            GetContainer()->RemoveChild(this);
        } else {
            // We don't have a container, so we we do this ourselves
            _doDestroy();
        }

        // Delete ourselves
        delete this;
    }

    bool Entity::RemoveComponent(const std::string &name_) {
        auto comp = GetComponent<Component>(name_);

        if (comp != nullptr) {
            // Remove component from map
            m_components.erase(name_);

            // Delete if we should
            delete comp;

            return true;
        }

        // We don't have this component
        return false;
    }

    std::vector<Component *> Entity::GetComponents() {
        std::vector<Component *> vec;

        for (auto &_Component : m_components) {
            vec.push_back(_Component.second);
        }

        return vec;
    }

    bool Entity::HasComponent(const std::string &name_) const {
        return m_components.find(name_) != m_components.end();
    }

    bool Entity::CanCull() const {
        return m_canCull;
    }

    void Entity::SetCanCull(bool canCull_) {
        m_canCull = canCull_;
    }

    bool Entity::CheckForCulling(Rectangle cullArea_) {
        return !cullArea_.Contains(GetPosition());
    }

    EntityContainer *Entity::GetContainer() const {
        if (m_parentEntity != nullptr)
            return static_cast<EntityContainer *>(m_parentEntity);
        else if (m_parentScene != nullptr)
            return static_cast<EntityContainer *>(m_parentScene);
        return nullptr;
    }

    Game *Entity::GetGame() const {
        if (m_parentScene == nullptr) return nullptr;
        return m_parentScene->GetGame();
    }

    Filesystem::ResourceManager *Entity::GetResourceManager() const {
        if (m_parentScene == nullptr) return nullptr;
        return m_parentScene->GetResourceManager();
    }

    int Entity::GetDepth() const {
        return m_depth;
    }

    void Entity::SetDepth(int depth_) {
        if (m_parentScene != nullptr)
            m_parentScene->_updateEntityDepth(depth_, this);
        m_depth = depth_;
    }

    Transform2D Entity::GetTransform() const {
        if (m_physicsEnabled) {
            return m_physicsBody->GetTransform();
        } else {
            return m_transform;
        }
    }

    void Entity::SetTransform(const Transform2D &transform_) {
        if (m_physicsEnabled) {
            m_physicsBody->SetTransform(transform_);
        } else {
            m_transform = transform_;
        }
    }

    Vector2 Entity::GetPosition() const {
        if (m_physicsEnabled) {
            return m_physicsBody->GetPosition();
        } else {
            return m_transform.Position;
        }
    }

    void Entity::SetPosition(const Vector2 position_) {
        if (m_physicsEnabled) {
            m_physicsBody->SetPosition(position_);
        } else {
            m_transform.Position = position_;
        }

        // Fire event
        OnTransformChanged(GetTransform());
    }

    void Entity::MoveBy(const Vector2 moveBy_) {
        if (m_physicsEnabled) {
            // Not recommended, read warning in docs.
            auto p = m_physicsBody->GetPosition();
            m_physicsBody->SetPosition(p + moveBy_);
        } else {
            m_transform.Position += moveBy_;
        }

        // Fire event
        OnTransformChanged(GetTransform());
    }

    Angle Entity::GetRotation() const {
        if (m_physicsBody == nullptr) {
            return m_transform.Rotation;
        } else {
            return m_physicsBody->GetTransform().Rotation;
        }
    }

    void Entity::SetRotation(Angle rotation_) {
        if (m_physicsBody == nullptr) {
            // Set our rotation
            m_transform.Rotation = rotation_;
        } else {
            // Set the body rotation
            m_physicsBody->SetRotation(rotation_);
        }

        // Fire event
        OnTransformChanged(GetTransform());
    }

    bool Entity::IsPhysicsEnabled() const {
        return m_physicsBody != nullptr;
    }

    Physics::PhysicsBody *Entity::GetPhysicsBody() {
        return m_physicsBody;
    }

    const Physics::PhysicsBody *Entity::GetPhysicsBody() const {
        return m_physicsBody;
    }

    Physics::PhysicsWorld *Entity::GetPhysicsWorld() {
        if (m_physicsBody == nullptr) return nullptr;
        return m_physicsBody->GetWorld();
    }

    const Physics::PhysicsWorld *Entity::GetPhysicsWorld() const {
        if (m_physicsBody == nullptr) return nullptr;
        return m_physicsBody->GetWorld();
    }

    bool Entity::GetDoPersistentUpdates() const {
        return m_persistentUpdates;
    }

    void Entity::SetDoPersistentUpdates(bool persistentUpdates_) {
        if (m_onUpdateRef.IsAttached())
            throw std::runtime_error("This property cannot be changed once update has been subscribed.");
        m_persistentUpdates = persistentUpdates_;
    }

    bool Entity::SubscribeToUpdate() {
        if (m_parentScene != nullptr) {
            if (!m_onUpdateRef.IsAttached()) {
                if (m_persistentUpdates)
                    m_onUpdateRef = m_parentScene->OnPersistentUpdate
                            += new ClassMethodEventHandler<Entity>(this, &Entity::Update);
                else
                    m_onUpdateRef = m_parentScene->OnUpdate
                            += new ClassMethodEventHandler<Entity>(this, &Entity::Update);
                return true;
            } else {
                // The event already exists, so return true anyway to avoid errors.
                return true;
            }
        }
        return false;
    }

    bool Entity::SubscribedToUpdate() const {
        if (m_parentScene == nullptr) return false;
        return m_onUpdateRef.IsAttached();
    }

    void Entity::UnsubscribeFromUpdate() {
        // Detach using the reference.
        m_onUpdateRef.Detach();
    }

    void Entity::Draw(Graphics::Renderer *renderer_) {
        // Trigger draw
        OnDraw(renderer_);

        // Debug draw
        if (m_physicsBody != nullptr) {
            if (m_physicsBody->GetWorld()->DebugDraw) {
                m_physicsBody->DebugDraw(renderer_);
            }
        }
    }

    void Entity::Update() {
        // Fire update event
        OnUpdate();

        if (m_physicsBody != nullptr) {
            // As we are a physics entity, fire this each frame
            OnTransformChanged(GetTransform());
        }
    }
}
