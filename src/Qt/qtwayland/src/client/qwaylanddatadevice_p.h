/****************************************************************************
**
** Copyright (C) 2016 Klarälvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtWaylandClient module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/


#ifndef QWAYLANDDATADEVICE_H
#define QWAYLANDDATADEVICE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <qtwaylandclientglobal.h>
#include <QObject>
#include <QPointer>
#include <QPoint>

#include <QtWaylandClient/private/qwayland-wayland.h>

#if QT_CONFIG(draganddrop)

QT_BEGIN_NAMESPACE

class QMimeData;
class QWindow;

namespace QtWaylandClient {

class QWaylandDisplay;
class QWaylandDataDeviceManager;
class QWaylandDataOffer;
class QWaylandDataSource;
class QWaylandInputDevice;
class QWaylandWindow;

class QWaylandDataDevice : public QObject, public QtWayland::wl_data_device
{
    Q_OBJECT
public:
    QWaylandDataDevice(QWaylandDataDeviceManager *manager, QWaylandInputDevice *inputDevice);
    ~QWaylandDataDevice();

    QWaylandDataOffer *selectionOffer() const;
    void invalidateSelectionOffer();
    QWaylandDataSource *selectionSource() const;
    void setSelectionSource(QWaylandDataSource *source);

    QWaylandDataOffer *dragOffer() const;
    void startDrag(QMimeData *mimeData, QWaylandWindow *icon);
    void cancelDrag();

protected:
    void data_device_data_offer(struct ::wl_data_offer *id) Q_DECL_OVERRIDE;
    void data_device_drop() Q_DECL_OVERRIDE;
    void data_device_enter(uint32_t serial, struct ::wl_surface *surface, wl_fixed_t x, wl_fixed_t y, struct ::wl_data_offer *id) Q_DECL_OVERRIDE;
    void data_device_leave() Q_DECL_OVERRIDE;
    void data_device_motion(uint32_t time, wl_fixed_t x, wl_fixed_t y) Q_DECL_OVERRIDE;
    void data_device_selection(struct ::wl_data_offer *id) Q_DECL_OVERRIDE;

private Q_SLOTS:
    void selectionSourceCancelled();
    void dragSourceCancelled();
    void dragSourceTargetChanged(const QString &mimeType);

private:
    QPoint calculateDragPosition(int x, int y, QWindow *wnd) const;

    QWaylandDisplay *m_display;
    QWaylandInputDevice *m_inputDevice;
    uint32_t m_enterSerial;
    QPointer<QWindow> m_dragWindow;
    QPoint m_dragPoint;
    QScopedPointer<QWaylandDataOffer> m_dragOffer;
    QScopedPointer<QWaylandDataOffer> m_selectionOffer;
    QScopedPointer<QWaylandDataSource> m_selectionSource;

    QScopedPointer<QWaylandDataSource> m_dragSource;
};

}

QT_END_NAMESPACE

#endif  // draganddrop

#endif // QWAYLANDDATADEVICE_H
