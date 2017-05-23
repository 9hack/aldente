#include "construct_preview.h"
#include "construct.h"

void ConstructPreview::set_construct_type(ConstructType type, bool valid) {
    // Remove visibility of previous preview, if any
    remove_child(curr_preview);

    // Call make_preview if never made preview before
    if (cached_previews.find(type) == cached_previews.end()) {
        switch (type) {
            case CHEST:
                cached_previews[type] = make_preview<Chest>();
                break;
            case SPIKES:
                cached_previews[type] = make_preview<Spikes>();
                break;
            default:
                cached_previews[type] = make_preview<Chest>();
                break;
        }
    }

    // Set curr preview to be visibile
    curr_preview = cached_previews[type];
    set_valid(valid);
    add_child(curr_preview);
}

template <typename T>
T* ConstructPreview::make_preview() {
    T *construct_preview = new T(0, 0, CLIENT_ONLY_ID);
    construct_preview->setup_model();
    construct_preview->set_filter_alpha(0.5f);
    return construct_preview;
}

void ConstructPreview::set_valid(bool valid) {
    this->valid = valid;
    curr_preview->set_filter_color(valid ? Color::GREEN : Color::RED);
}

bool ConstructPreview::get_valid() const {
    return valid;
}
