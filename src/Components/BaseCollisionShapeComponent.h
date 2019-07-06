/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*   
*   File reviewed on 01/06/2019 by R.M
*
**********************************************************************************************/

#ifndef BASECOLLISIONSHAPECOMPONENT_H
#define BASECOLLISIONSHAPECOMPONENT_H

#include "../ngine.h"

#include "../Core/BaseEntity.h"
#include "../Core/Component.h"

namespace NerdThings::Ngine::Components {
    /*
     * Base Collision Shape Component
     */
    class BaseCollisionShapeComponent : public Core::Component {
        // Private Fields

        /*
         * Collision group used
         */
        std::vector<std::string> _CollisionGroups = {"General"};

        /*
         * Whether or not to draw debug geometry
         */
        bool _DebugDraw = false;

        /*
         * On position changed
         */
        Core::EventHandleRef<EntityTransformChangedEventArgs> _OnTransformChangeRef;

        // Private Methods

        /*
         * Check for a collision
         */
        virtual bool CollisionCheck(BaseCollisionShapeComponent *b) = 0;

        /*
         * Draw debug geometry
         */
        virtual void DrawDebug() = 0;

        /*
         * Whether or not a shape is compatible.
         * This simple addition allows custom shapes to be added
         */
        virtual bool IsCompatible(BaseCollisionShapeComponent *b) = 0;

        /*
         * Offset shape
         */
        virtual void Offset(Math::TVector2 offset_) = 0;

        /*
         * Update shape information
         */
        virtual void UpdateShape(EntityTransformChangedEventArgs &e) = 0;
    public:
        // Destructor

        virtual ~BaseCollisionShapeComponent() {
            _OnTransformChangeRef.UnBind();
        }

        // Public Methods

        /*
         * Add a collision group
         */
        void AddCollisionGroup(const std::string collisionGroup_) {
            _CollisionGroups.push_back(collisionGroup_);

            // Add to scene
            auto scene = GetParent<Core::BaseEntity>()->GetParentScene();
            if (scene->CollisionMap.find(collisionGroup_) == scene->CollisionMap.end()) {
                scene->CollisionMap[collisionGroup_] = std::vector<Core::BaseEntity*>();
            }
            scene->CollisionMap[collisionGroup_].push_back(GetParent<Core::BaseEntity>());
        }

        /*
         * Check for a collision
         */
        template <typename EntityType>
        bool CheckCollision() {
            return CheckCollisionAt<EntityType>(GetParent<Core::BaseEntity>()->GetPosition());
        }

        /*
         * Check for a collision at a position
         */
        template <typename EntityType>
        bool CheckCollisionAt(Math::TVector2 position_) {
            auto curPos = GetParent<Core::BaseEntity>()->GetPosition();
            auto diff = curPos - position_;

            // Offset shape
            Offset(diff);

            // Check for collision
            auto collision = false;

            auto scene = GetParent<Core::BaseEntity>()->GetParentScene();

            for (auto group : GetCollisionGroups()) {
                auto candidates = scene->CollisionMap[group];

                for (auto candidate : candidates) {
                    if (candidate == GetParent<Core::BaseEntity>())
                        continue;

                    auto components = candidate->GetComponents();

                    for (auto component : components) {
                        auto colShapeComp = dynamic_cast<BaseCollisionShapeComponent*>(component);
                        if (colShapeComp != nullptr) {
                            if (IsCompatible(colShapeComp)) {
                                collision = CollisionCheck(colShapeComp);
                            } else if (colShapeComp->IsCompatible(this)) {
                                collision = colShapeComp->CollisionCheck(this);
                            }

                            if (collision) break;
                        }
                    }

                    if (collision) break;
                }

                if (collision) break;
            }

            // Un-offset shape
            Offset({-diff.X, -diff.Y});

            return collision;
        }

        /*
         * Check for a collision with a collision group.
         * Component must have the collision group to work
         */
        template <typename EntityType>
        bool CheckCollisionWith(const std::string &collisionGroup_) {
            return CheckCollisionWithAt<EntityType>(collisionGroup_, GetParent<Core::BaseEntity>()->GetPosition());
        }

        /*
         * Check for a collision with a collision group at a position
         * Component must have the collision group to work
         */
        template <typename EntityType>
        bool CheckCollisionWithAt(const std::string &collisionGroup_, Math::TVector2 position_) {
            auto curPos = GetParent<Core::BaseEntity>()->GetPosition();
            auto diff = curPos - position_;

            // Offset shape
            Offset(diff);

            // Check for collision
            auto collision = false;

            auto scene = GetParent<Core::BaseEntity>()->GetParentScene();

            if (scene->CollisionMap.find(collisionGroup_) != scene->CollisionMap.end()) {
                auto candidates = scene->CollisionMap[collisionGroup_];

                for (auto candidate : candidates) {
                    auto components = candidate->GetComponents();

                    for (auto component : components) {
                        auto colShapeComp = dynamic_cast<BaseCollisionShapeComponent*>(component);
                        if (colShapeComp != nullptr) {
                            collision = CollisionCheck(colShapeComp);

                            if (collision) break;
                        }
                    }

                    if (collision) break;
                }
            }

            // Un-offset shape
            Offset({-diff.X, -diff.Y});

            return collision;
        }

        /*
         * Disable debug geometry drawing
         */
        void DisableDebugDraw() {
            if (_DebugDraw) {
                _DebugDraw = false;

                UnsubscribeFromDraw();
            }
        }

        void Draw(Core::EventArgs &e) override {
            if (_DebugDraw) {
                DrawDebug();
            }
        }

        /*
         * Enable debug geometry drawing
         */
        void EnableDebugDraw() {
            if (!_DebugDraw) {
                SubscribeToDraw();

                _DebugDraw = true;
            }
        }

        /*
         * Get the set collision group
         */
        std::vector<std::string> GetCollisionGroups() {
            return _CollisionGroups;
        }

        /*
         * Remove a collision group
         */
        void RemoveCollisionGroup(std::string collisionGroup_) {
            _CollisionGroups.erase(std::remove(_CollisionGroups.begin(), _CollisionGroups.end(), collisionGroup_),
                                   _CollisionGroups.end());

            // Remove from scene
            auto scene = GetParent<Core::BaseEntity>()->GetParentScene();
            if (scene->CollisionMap.find(collisionGroup_) != scene->CollisionMap.end()) {
                auto colVec = scene->CollisionMap[collisionGroup_];
                colVec.erase(std::remove(colVec.begin(), colVec.end(), GetParent<Core::BaseEntity>()), colVec.end());
            }
        }

    protected:

        // Protected Constructor(s)

        /*
         * Create a base collision shape component.
         */
        BaseCollisionShapeComponent(Core::BaseEntity *parent_, std::string collisionGroup_ = "General")
            : Component(parent_) {
            _OnTransformChangeRef = GetParent<Core::BaseEntity>()->OnTransformChanged.Bind(
                this, &BaseCollisionShapeComponent::UpdateShape);

            AddCollisionGroup(std::move(collisionGroup_));
        }
    };
}

#endif //BASECOLLISIONSHAPECOMPONENT_H
