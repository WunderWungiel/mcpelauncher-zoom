#include "zoom.h"
#include "main.h"
#include "util.h"
#include "conf.h"

namespace {
// Zoomify defaults: 1.0s zoom in, 0.5s zoom out, both ease-out-exponential.
constexpr int zoomInDurationMs = 1000;
constexpr int zoomOutDurationMs = 500;
// Scroll steps use the same easing but shorter durations so wheel input stays responsive.
constexpr int scrollInDurationMs = 250;
constexpr int scrollOutDurationMs = 150;
}

unsigned long Zoom::CameraAPI_tryGetFOV(void* t) {
    lastClientZoom = CameraAPI_tryGetFOV_orig(t);
    if(!Conf::animated) {
        return zoomKeyDown ? zoomLevel : lastClientZoom;
    }
    if(transition.inProgress() || zoomKeyDown) {
        transition.tick(getEpochTime());
        unsigned long current = transition.getCurrent();
        if(current == 0) {
            return lastClientZoom;
        }
        return current;
    }
    return lastClientZoom;
}

bool Zoom::onMouseScroll(double dy) {
    if(zoomKeyDown && game_window_is_mouse_locked(game_window_get_primary_window())) {
        if(dy > 0) {
            if(zoomLevel > 5310000000) {
                if(Conf::animated) {
                    transition.startTransition(zoomLevel, zoomLevel - 1000000, scrollInDurationMs, getEpochTime());
                }
                zoomLevel -= 1000000;
            }
        } else if(zoomLevel < 5360000000) {
            if(Conf::animated) {
                transition.startTransition(zoomLevel, zoomLevel + 1000000, scrollOutDurationMs, getEpochTime());
            }
            zoomLevel += 1000000;
        }
        return true;
    }
    return false;
}
void Zoom::onKeyboard(int keyCode, int action) {
    if(keyCode == Conf::zoomKey) {
        switch(action) {
        case 0:
            if(game_window_is_mouse_locked(game_window_get_primary_window()) && !zoomKeyDown) {
                zoomKeyDown = true;
                if(Conf::animated) {
                    transition.startTransition(lastClientZoom, zoomLevel, zoomInDurationMs, getEpochTime());
                }
            }
            break;
        case 2:
            if(zoomKeyDown) {
                zoomKeyDown = false;
                if(Conf::animated) {
                    transition.startTransition(zoomLevel, lastClientZoom, zoomOutDurationMs, getEpochTime());
                }
            }
        default:
            break;
        }
    }
}
