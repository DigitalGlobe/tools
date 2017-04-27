/****************************************************************************
**
** Copyright (C) 2013 Klarälvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtWaylandCompositor module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QWAYLANDDRAG_H
#define QWAYLANDDRAG_H

#include <QtWaylandCompositor/qwaylandexport.h>

#include <QtCore/QObject>
#include <QtCore/QPointF>

QT_BEGIN_NAMESPACE

class QWaylandDragPrivate;
class QWaylandSurface;
class QWaylandView;
class QWaylandInputDevice;

namespace QtWayland {
    class DataDevice;
}

class Q_WAYLAND_COMPOSITOR_EXPORT QWaylandDrag : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(QWaylandDrag)

    Q_PROPERTY(QWaylandSurface *icon READ icon NOTIFY iconChanged)
    Q_PROPERTY(bool visible READ visible NOTIFY iconChanged)

public:
    explicit QWaylandDrag(QWaylandInputDevice *inputDevice);

    QWaylandSurface *icon() const;
    // QPointF position() const;
    bool visible() const;

public Q_SLOTS:
    void dragMove(QWaylandSurface *target, const QPointF &pos);
    void drop();
    void cancelDrag();

Q_SIGNALS:
    void iconChanged();
    void dragStarted(); // QWaylandSurface *icon????

private:
    //friend class QtWayland::DataDevice;
};

QT_END_NAMESPACE

#endif // QWAYLANDDRAG_H
