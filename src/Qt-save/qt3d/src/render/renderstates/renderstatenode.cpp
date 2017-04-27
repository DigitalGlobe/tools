/****************************************************************************
**
** Copyright (C) 2016 Klaralvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
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

#include "renderstatenode_p.h"
#include <Qt3DRender/qrenderstate.h>
#include <Qt3DRender/private/qrenderstatecreatedchange_p.h>
#include <Qt3DCore/qpropertyupdatedchange.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {
namespace Render {

RenderStateNode::RenderStateNode()
    : BackendNode()
    , m_impl(NULL)
{
}

RenderStateNode::~RenderStateNode()
{
    cleanup();
}

void RenderStateNode::cleanup()
{
    if (m_impl != nullptr) {
        if (!m_impl->isPooledImpl())
            delete m_impl;
        m_impl = nullptr;
    }
}

void RenderStateNode::initializeFromPeer(const Qt3DCore::QNodeCreatedChangeBasePtr &change)
{
    cleanup();
    const auto renderStateChange = qSharedPointerCast<Qt3DRender::QRenderStateCreatedChangeBase>(change);
    m_impl = RenderStateImpl::getOrCreateState(renderStateChange);
}

void RenderStateNode::sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e)
{
    if (e->type() == Qt3DCore::PropertyUpdated) {
        Qt3DCore::QPropertyUpdatedChangePtr propertyChange = qSharedPointerCast<Qt3DCore::QPropertyUpdatedChange>(e);

        if (m_impl->isPooledImpl()) {
            m_impl = m_impl->getOrCreateWithPropertyChange(propertyChange->propertyName(), propertyChange->value());
        } else {
            m_impl->updateProperty(propertyChange->propertyName(), propertyChange->value());
        }
        markDirty(AbstractRenderer::AllDirty);
    }
    BackendNode::sceneChangeEvent(e);
}

} // namespace Render
} // namespace Qt3DRender

QT_END_NAMESPACE
