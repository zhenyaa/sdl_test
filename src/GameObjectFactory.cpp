//
// Created by zhenya on 17.02.2026.
//

#include "GameObjectFactory.h"

#include <memory>

#include "PrefabProcessor.h"
#include "SDL3_image/SDL_image.h"

// bool GameObjectFactory::spawn(GameManager *gm, const std::string &prefab, float x, float y) {
//     auto prefab = PrefabProcessor::get(prefab);
//     if (!prefab) return nullptr;
//
//     // создаём объект
//     auto obj = std::make_unique<GameObject>(x, y, 2.f, prefab->colliders);
//
//     // загружаем текстуру (пока просто как пример)
//     obj->sprite.texture = IMG_LoadTexture(gm->renderer, prefab->spritePath.c_str());
//     if (!obj->sprite.texture) {
//         SDL_Log("Failed to load texture for prefab %s: %s", prefabName.c_str(), SDL_GetError());
//         return nullptr;
//     }
//
//     obj->sprite.src = {
//         static_cast<float>(prefab->sprite.x),
//         static_cast<float>(prefab->sprite.y),
//         static_cast<float>(prefab->sprite.w),
//         static_cast<float>(prefab->sprite.h)
//     };
//
//     // добавляем в GameManager
//     gm->platforms.push_back(std::move(obj));
//
//     // возвращаем raw pointer на объект
//     return gm->platforms.back().get();
// }
