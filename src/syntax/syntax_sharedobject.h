#ifndef SYNTAX_SHAREDOBJECT_H
#define SYNTAX_SHAREDOBJECT_H

#   include "method_slot_peer.h"
#   include "decl_signal_peer.h"
//#ifdef NEURON_CLIENT
//#else
//#   include "method_slot_sharedobject.h"
//#   include "decl_signal_sharedobject.h"
//#endif

#include "syntax.h"

#ifdef N_CLASS_IMPL
#undef N_CLASS_IMPL
#endif


#define N_CLASS_IMPL(class)                                                     \
        class::class(QObject *parent) : NEURON_WRAP_NAMESPACE(SharedObject)(parent)     \
        {                                                                       \
            setPeerName(#class);                                                \
            initalize();                                                        \
        }                                                                       \
        class::class(NEURON_WRAP_NAMESPACE(AbstractHub) *hub, QObject *parent)  \
                : NEURON_WRAP_NAMESPACE(Peer)(parent)                           \
        {                                                                       \
            setPeerName(#class);                                                \
            if(hub)                                                             \
                setHub(hub);                                                    \
            initalize();                                                        \
        }                                                                       \
        void class::initalize()

#endif // SYNTAX_SHAREDOBJECT_H
