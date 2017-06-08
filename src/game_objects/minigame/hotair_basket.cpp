#include "hotair_basket.h"
#include "asset_loader.h"
#include "events.h"

HotairBasket(int id) : GameObject(-1) {
    tag = "BASKET";
}

void HotairBasket::setup_model() {
    attach_model(AssetLoader::get_model("basket"));
    transform.set_scale({ 0.006f, 0.006f, 0.006f });
    initial_transform.set_scale(transform.get_scale());
}