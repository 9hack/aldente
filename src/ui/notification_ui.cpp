#include <events.h>
#include <timer.h>
#include <iostream>
#include "notification_ui.h"

NotificationUI::NotificationUI(float aspect, float width, float height)
    : aspect_width(aspect * width), height(height), animating(false) {

    // Show notification when requested
    events::ui::show_notification.connect([&](const std::string &message) {
        queue.push(message);
        show_next();
    });
}

std::shared_ptr<UIUnstretchedTextBox> NotificationUI::make_notification(std::string text) {
    // Create a notification off-screen
    std::shared_ptr<UIUnstretchedTextBox> notif = std::make_shared<UIUnstretchedTextBox>(
            3.f, 4.f, 0, -height, aspect_width, height,
            2.f, UIUnstretchedTextBox::START, UIUnstretchedTextBox::START,
            Color::WHITE, Color::BLACK, .5f
    );
    notif->set_text(text);
    attach(*notif);
    notifications.insert(notif);

    // Fade it out and remove it after 1 second
    Timer::get()->do_after(std::chrono::seconds(1), [&, notif]() {
        notif->animate_alpha(0, .5f, [&, notif]() {
            detach(*notif);
            notifications.erase(notif);
            graveyard.insert(notif);
        });
    });

    return notif;
}

void NotificationUI::show_next() {
    if (animating || queue.empty()) return;

    auto notif = make_notification(queue.front());
    queue.pop();

    // Shift all notifications up by height, then bring in the next notification
    // I sure am glad we're single-threaded
    animating = true;
    for (auto cur : notifications) {
        // Once the most recent notification has been shown, bring in the next notification
        std::function<void()> callback = [](){};
        if (cur == notif)
            callback = [&]() {
                animating = false;
                show_next();
            };

        cur->animate_relative(0, height, .2f, callback);
    }
}