#ifndef MESSAGESTUFF_H
#define MESSAGESTUFF_H

#include <gloox/messagesession.h>
#include <gloox/messageeventfilter.h>
#include <gloox/chatstatefilter.h>

struct MessageStuff {
	gloox::MessageSession *session;
	gloox::MessageEventFilter *eventFilter;
	gloox::ChatStateFilter *stateFilter;
    bool chatWindowOpen;
};

#endif // MESSAGESTUFF_H
