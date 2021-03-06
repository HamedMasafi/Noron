/**************************************************************************
**
** This file is part of .
** https://github.com/HamedMasafi/
**
**  is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
**  is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with .  If not, see <http://www.gnu.org/licenses/>.
**
**************************************************************************/

#include <QtCore/QMetaObject>
#include <QtCore/QDebug>

#include "peer.h"
#include "abstracthub.h"
#include "sharedobject.h"

NEURON_BEGIN_NAMESPACE

/*!
 * \brief Peer
 * This is base class of all peers.
 */

Peer::Peer(QObject *parent)
    : QObject(parent), _peerName(QString::null), m_hub(0)
{
}

Peer::Peer(AbstractHub *hub, QObject *parent)
    : QObject(parent), _peerName(QString::null), m_hub(0)
{
    setHub(hub);
}

Peer::~Peer()
{
    //    if(hub())
    //        hub()->deleteLater();
}

AbstractHub *Peer::hub() const
{
    return m_hub;
}

qlonglong Peer::invokeOnPeer(QString methodName, QVariant val0,
                                  QVariant val1, QVariant val2, QVariant val3,
                                  QVariant val4, QVariant val5, QVariant val6,
                                  QVariant val7, QVariant val8, QVariant val9)
{
    // Object is transfered!!!
    if (!hub())
        return 0;

    if (hub()->status() != AbstractHub::Connected)
        return 0;

    if (hub()->isMultiThread()) {
        //        qlonglong ret;
        bool ok = hub()->metaObject()->invokeMethod(
            hub(), QT_STRINGIFY(invokeOnPeer),
            //                                          Qt::DirectConnection,
            //                                          Q_RETURN_ARG(qlonglong,
            //                                          ret),
            Q_ARG(QString, peerName()), Q_ARG(QString, methodName),
            Q_ARG(QVariant, val0), Q_ARG(QVariant, val1), Q_ARG(QVariant, val2),
            Q_ARG(QVariant, val3), Q_ARG(QVariant, val4), Q_ARG(QVariant, val5),
            Q_ARG(QVariant, val6), Q_ARG(QVariant, val7) /*,
                                                                        QGenericArgument(val8.typeName(), val8.data()),
                                                                        QGenericArgument(val9.typeName(), val9.data())*/);
        //        return ret;
        if (!ok)
            qWarning("Unable to invoke method: %s::%s", qPrintable(peerName()),
                     qPrintable(methodName));
        return 0;
    } else {
        return hub()->invokeOnPeer(metaObject()->className(), methodName, val0,
                                   val1, val2, val3, val4, val5, val6, val7,
                                   val8, val9);
    }
}

const QString Peer::peerName() const
{
    return _peerName;
}

void Peer::addCall(qlonglong id, AbstractCall *call)
{
    hub()->_calls.insert(id, call);
}

void Peer::removeCall(qlonglong id)
{
    delete hub()->_calls[id];
    hub()->_calls.remove(id);
}

void Peer::setPeerName(const QString &name)
{
    _peerName = name;
}

void Peer::hub_disconnected()
{
    m_hub = nullptr;
}

void Peer::setHub(AbstractHub *hub)
{
    if (m_hub == hub)
        return;

    if (m_hub) {
        disconnect(m_hub, &AbstractHub::disconnected,
                   this, &Peer::hub_disconnected);
    }
    m_hub = hub;

    if (hub) {
        connect(hub, &AbstractHub::disconnected,
                this, &Peer::hub_disconnected);

        SharedObject *so = qobject_cast<SharedObject *>(this);

        if (so)
            hub->attachSharedObject(so);
        else
            hub->setPeer(this);

        emit hubChanged(hub);
    }
}

NEURON_END_NAMESPACE
