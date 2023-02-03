/*
  This file is part of the Ofi Labs X2 project.

  Copyright (C) 2010 Ariya Hidayat <ariya.hidayat@gmail.com>

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "kineticmodel.h"

#include <QTimer>
#include <QDateTime>

#include <math.h>

class KineticModelPrivate
{
public:
    QTimer ticker;

    int duration;
    qreal position;

    QTime timestamp;
    qreal lastPosition;
    qreal velocity;
    bool released;
    qreal targetPosition;

    KineticModelPrivate();
};

KineticModelPrivate::KineticModelPrivate()
    : duration(2604)
    , position(0)
    , lastPosition(0)
    , velocity(0)
    , released(false)
    , targetPosition(0)
{
}

KineticModel::KineticModel(QObject *parent)
    : QObject(parent)
    , d_ptr(new KineticModelPrivate)
{
    connect(&d_ptr->ticker, SIGNAL(timeout()), SLOT(update()));
    d_ptr->ticker.setInterval(1000 / 60); // default
}

KineticModel::~KineticModel()
{
}

int KineticModel::duration() const
{
    return d_ptr->duration;
}

void KineticModel::setDuration(int ms)
{
    d_ptr->duration = ms;
}

qreal KineticModel::position() const
{
    return d_ptr->position;
}

void KineticModel::setPosition(qreal pos)
{
    qreal oldPos = d_ptr->position;
    d_ptr->position = pos;
    if (!qFuzzyCompare(pos, oldPos))
        emit positionChanged(pos);

    update();
    if (!d_ptr->ticker.isActive())
        d_ptr->ticker.start();
}

int KineticModel::updateInterval() const
{
    return d_ptr->ticker.interval();
}

void KineticModel::setUpdateInterval(int ms)
{
    d_ptr->ticker.setInterval(ms);
}

void KineticModel::resetSpeed()
{
    Q_D(KineticModel);

    d->released = false;
    d->ticker.stop();
    d->lastPosition = d->position;
    d->timestamp.start();
    d->velocity = 0;
}

void KineticModel::release()
{
    Q_D(KineticModel);

    d->targetPosition = d->position + d->velocity;
    d->timestamp.start();
    d->released = true;

    if (d->velocity > 10 || d->velocity < -10) {
        if (!d->ticker.isActive())
            d->ticker.start();
        update();
    } else {
        d->ticker.stop();
    }
}

void KineticModel::update()
{
    Q_D(KineticModel);

    if (d->released) {
        qreal timeConstant = d->duration / 6;
        qreal delta = d->velocity * ::exp(-d->timestamp.elapsed() / timeConstant);
        d->position = d->targetPosition - delta;
        if (d->timestamp.elapsed() > d->duration) {
            d->velocity = 0;
            d->ticker.stop();
        }
        emit positionChanged(d->position);
    } else {
        int elapsed = d->timestamp.elapsed();

        // too fast gives less accuracy
        if (elapsed > d->ticker.interval() / 2) {
            qreal delta = static_cast<qreal>(elapsed) / 1000.0;
            d->timestamp.start();

            qreal lastSpeed = d->velocity;
            qreal currentSpeed = (d->position - d->lastPosition) / delta;
            d->velocity = .2 * lastSpeed + .8 * currentSpeed;
            d->lastPosition = d->position;
        }
    }
}
