#ifndef HALLSOFBUBBA_EVENTHANDLER_H
#define HALLSOFBUBBA_EVENTHANDLER_H

#include <functional>
#include <unordered_map>

typedef std::function<void()> eventListener_t;
typedef int eventListenerIdentifier_t;

class EventHandler {

public:

    static void fire(int event);
    static eventListenerIdentifier_t subscribe(int event, eventListener_t listener);
    static void unsubscribe(eventListenerIdentifier_t listenerIdentifier);

private:

    struct eventListener_s {
        eventListener_t callback;
        int identifier;
        eventListener_s(int identifier, eventListener_t listener);
    };

    static eventListenerIdentifier_t nextIdentifier;

    static std::unordered_multimap<int, eventListener_s> eventListeners;
    static std::unordered_map<eventListenerIdentifier_t, int> eventListenedTo;

};


#endif //HALLSOFBUBBA_EVENTHANDLER_H
