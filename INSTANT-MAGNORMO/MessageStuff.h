#ifndef MESSAGESTUFF_H
#define MESSAGESTUFF_H

#include <gloox/messagesession.h>
#include <gloox/messageeventfilter.h>
#include <gloox/chatstatefilter.h>

struct MessageStuff {
    MessageSession *session;
    MessageEventFilter *eventFilter;
    ChatStateFilter *stateFilter;
};

#endif // MESSAGESTUFF_H
