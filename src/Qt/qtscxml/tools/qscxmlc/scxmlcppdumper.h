/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtScxml module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef CPPDUMPER_H
#define CPPDUMPER_H

#include "qscxmlglobals.h"

#include <QtScxml/private/qscxmlparser_p.h>

#include <QTextStream>

QT_BEGIN_NAMESPACE

struct ClassDump;

struct TranslationUnit
{
    TranslationUnit()
        : useCxx11(true)
        , mainDocument(Q_NULLPTR)
    {}

    QString scxmlFileName;
    QString outHFileName, outCppFileName;
    QString namespaceName;
    bool useCxx11;
    DocumentModel::ScxmlDocument *mainDocument;
    QHash<DocumentModel::ScxmlDocument *, QString> classnameForDocument;
    QList<TranslationUnit *> dependencies;

    QList<DocumentModel::ScxmlDocument *> otherDocuments() const
    {
        auto docs = classnameForDocument.keys();
        docs.removeOne(mainDocument);
        return docs;
    }
};

class CppDumper
{
public:
    CppDumper(QTextStream &headerStream, QTextStream &cppStream)
        : h(headerStream)
        , cpp(cppStream)
    {}

    void dump(TranslationUnit *unit);

private:
    void writeHeaderStart(const QString &headerGuard, const QStringList &forwardDecls);
    void writeClass(const ClassDump &clazz);
    void writeHeaderEnd(const QString &headerGuard, const QStringList &metatypeDecls);
    void writeImplStart(const QVector<ClassDump> &allClazzes);
    void writeImplBody(const ClassDump &clazz);
    void writeImplEnd();

private:
    QTextStream &h;
    QTextStream &cpp;

    static QByteArray b(const char *str) { return QByteArray(str); }
    static QLatin1String l (const char *str) { return QLatin1String(str); }

    TranslationUnit *m_translationUnit;
};

QT_END_NAMESPACE

#endif // CPPDUMPER_H
