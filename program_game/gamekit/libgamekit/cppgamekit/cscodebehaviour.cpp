#include "cscodebehaviour.hh"

cq_member(csCodeBehaviour) {
    bool waitingStart = true;
};

void csCodeBehaviour::emitAwake() {
    awake();
}

void csCodeBehaviour::emitUpdate() {
    //start.
    if (dat->waitingStart) {
        dat->waitingStart = false;
        start();
    }
    //update.
    update();
}

void csCodeBehaviour::emitDestroy() {
    onDestroy();
}

void csCodeBehaviour::awake    () {}
void csCodeBehaviour::start    () {}
void csCodeBehaviour::update   () {}
void csCodeBehaviour::onDestroy() {}
