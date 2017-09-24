#include "EventHandler.h"

std::unordered_multimap<int, EventHandler::eventListener_s> EventHandler::eventListeners;
std::unordered_map<eventListenerIdentifier_t, int> EventHandler::eventListenedTo;
eventListenerIdentifier_t EventHandler::nextIdentifier = 0;

eventListenerIdentifier_t EventHandler::subscribe(int event, eventListener_t listener) {
    int identifier = nextIdentifier++;
    eventListeners.insert(std::pair<int, eventListener_s>(event, eventListener_s(identifier, listener)));
    eventListenedTo.insert(std::pair<eventListenerIdentifier_t, int>(identifier, event));
    return identifier;
}



EventHandler::eventListener_s::eventListener_s(int identifier, eventListener_t callback)
    : identifier(identifier), callback(callback){
}

void EventHandler::unsubscribe(eventListenerIdentifier_t listenerIdentifier) {
    auto listeningOn = eventListenedTo.find(listenerIdentifier);
    if (listeningOn != eventListenedTo.end()) {
        auto listeners = eventListeners.equal_range(listeningOn->second);
        for(auto listener = listeners.first; listener != listeners.second; ) {
            if(listener->second.identifier == listenerIdentifier) {
                eventListeners.erase(listener);
            } else {
                listener++;
            }
        }
        eventListenedTo.erase(listeningOn);
    }
}

void EventHandler::fire(int event) {

    for(auto listener : eventListeners) {
        listener.second.callback();
    }

}
