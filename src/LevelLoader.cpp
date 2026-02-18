//
// Created by zhenya on 18.02.2026.
//

#include "LevelLoader.h"
#include <memory>

#include "GameManager.h"
#include "PrefabProcessor.h"
#include "GameObject.h"
#include "utils.h"
#include "SDL3/SDL_log.h"

/*
Prefab pref = PrefabProcessor::get("HoleLarge").value();
    std::unique_ptr<GameObject> gras = std::make_unique<GameObject>(900, 700, 1.5f, pref.colliders);
    Sprite s = Sprite(pref.sprite, SDL_FRect{0,0,160,158}, true); // матод SDL_RenderTexture не отрабатывает без источника так как выходной считаеться по нему
    gras->sprite = s;
    state->addTexture(std::move(gras));
*/
void LevelLoader::spawn(GameManager &gm, const std::string &prefab, float x, float y, float scale) {
    std::optional<Prefab> pref = PrefabProcessor::get(prefab);

    if (!pref) {
        SDL_Log("Prefab not found: %s", prefab.c_str());
        return;
    }

    auto obj = std::make_unique<GameObject>(
        x, y, scale, pref->colliders
    );
    obj->sprite = Sprite(pref->sprite, SDL_FRect{0,0,pref->width,pref->height}, false);



    // if (pref->hasRigidBody)
    //     obj->rb.;

    gm.addObject(std::move(obj));
}
