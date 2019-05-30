/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#ifndef BASECOLLISIONSHAPECOMPONENT_H
#define BASECOLLISIONSHAPECOMPONENT_H

#include <utility>
#include "../ngine.h"

#include "../Core/BaseEntity.h"
#include "../Core/Component.h"

namespace NerdThings::Ngine::Components {
    /*
     * Collision shape component
     */
    class BaseCollisionShapeComponent : public Core::Component {
        // Private Fields

        /*
         * Collision group used
         */
        std::vector<std::string> _CollisionGroups = { "General" };

        /*
         * Whether or not to draw debug geometry
         */
        bool _DebugDraw = false;

        /*
         * Whether or not drawing with the camera
         */
        bool _DebugDrawCamera = false;

        /*
         * On position changed
         */
        Core::EventHandleRef<EntityPositionChangedEventArgs> _OnPosChangeRef;

        // Private Methods

        virtual bool CollisionCheck(BaseCollisionShapeComponent* b) = 0;

        /*
         * Draw debug geometry
         */
        virtual void DrawDebug() = 0;

        /*
         * Offset shape
         */
        virtual void Offset(TVector2 offset_) = 0;

        /*
         * Update shape information
         */
        virtual void UpdateShape(EntityPositionChangedEventArgs &e) = 0;
    public:

        // Public Constructor(s)

        BaseCollisionShapeComponent(Core::BaseEntity* parent_, std::string collisionGroup_ =  "General" )
            : Component(parent_) {
            _OnPosChangeRef = GetParent<Core::BaseEntity>()->OnPositionChanged.Bind(this, &BaseCollisionShapeComponent::UpdateShape);

            AddCollisionGroup(std::move(collisionGroup_));
        }

        // Destructor

        virtual ~BaseCollisionShapeComponent() = default;

        // Public Methods

        void AddCollisionGroup(const std::string collisionGroup_) {
            _CollisionGroups.push_back(collisionGroup_);

            // Add to scene
            auto scene = GetParent<Core::BaseEntity>()->ParentScene;
            if (scene->CollisionMap.find(collisionGroup_) == scene->CollisionMap.end()) {
                scene->CollisionMap[collisionGroup_] = std::vector<Core::BaseEntity*>();
            }
            scene->CollisionMap[collisionGroup_].push_back(GetParent<Core::BaseEntity>());
        }

        template<typename EntityType>
        bool CheckCollision() {
            return CheckCollisionAt<EntityType>(GetParent<Core::BaseEntity>()->GetPosition());
        }

        template<typename EntityType>
        bool CheckCollisionAt(TVector2 position_) {
            auto curPos = GetParent<Core::BaseEntity>()->GetPosition();
            auto diff = curPos - position_;

            // Offset bounding box
            Offset(diff);

            // Check for collision
            auto collision = false;

            auto scene = GetParent<Core::BaseEntity>()->ParentScene;

            for (auto group : GetCollisionGroups()) {
                auto candidates = scene->CollisionMap[group];

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

                if (collision) break;
            }

            // Un-offset bounding box
            Offset({ -diff.X, -diff.Y });

            return collision;
        }

        void DisableDebugDraw() {
            if (_DebugDraw) {
                _DebugDraw = false;

                UnsubscribeFromDraw();
                UnsubscribeFromCameraDraw();
            }
        }

        void Draw(EventArgs &e) override {
            if (_DebugDraw) {
                if (!_DebugDrawCamera) {
                    DrawDebug();
                }
            }
        }

        void DrawCamera(EventArgs &e) override {
            if (_DebugDraw) {
                if (_DebugDrawCamera) {
                    DrawDebug();
                }
            }
        }

        void EnableDebugDraw(bool withCamera_ = false) {
            if (!_DebugDraw) {
                _DebugDrawCamera = withCamera_;

                if (_DebugDrawCamera) {
                    SubscribeToCameraDraw();
                } else {
                    SubscribeToDraw();
                }

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
            _CollisionGroups.erase(std::remove(_CollisionGroups.begin(), _CollisionGroups.end(), collisionGroup_), _CollisionGroups.end());

            // Remove from scene
            auto scene = GetParent<Core::BaseEntity>()->ParentScene;
            if (scene->CollisionMap.find(collisionGroup_) != scene->CollisionMap.end()) {
                auto colVec = scene->CollisionMap[collisionGroup_];
                colVec.erase(std::remove(colVec.begin(), colVec.end(), GetParent<Core::BaseEntity>()), colVec.end());
            }
        }
    };
}

#endif //BASECOLLISIONSHAPECOMPONENT_H
