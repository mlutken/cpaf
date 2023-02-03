/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "treemodel.h"

#include <QApplication>
#include <QFile>
#include <QTreeView>

#include "PSBDelegate.h"


#if 1
int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(simpletreemodel);

    QApplication app(argc, argv);

    QFile file(":/psb.txt");
    file.open(QIODevice::ReadOnly);
    TreeModel model(file.readAll());
    file.close();

    QTreeView view;
    view.setRootIsDecorated(false);
    view.setIndentation(0);
    view.setModel(&model);
    view.setWindowTitle(QObject::tr("Simple Tree Model"));

    view.setItemDelegate(new PSBDelegate);
    view.resize(500, 300);
    view.resizeColumnToContents(0);
    view.expandAll();
    view.show();
    return app.exec();
}

#else
#include <QtGui>
#include <QItemDelegate>

class ItemDelegate : public QItemDelegate
{
public:
    ItemDelegate()
    {}
    QSize sizeHint ( const QStyleOptionViewItem & /*option*/, const QModelIndex & /*index*/ ) const
    {
        return QSize(50,200);
    }
};

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    QTreeView view;

    QStandardItemModel model;
    model.insertRows(0, 4);
    model.insertColumns(0, 1);
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 1; col++) {
            QModelIndex index = model.index(row, col);
            model.setData(index, QVariant((row+1) *(col+1)).toString());
        }}


    //ItemDelegate *delegate = new ItemDelegate();
    PSBDelegate *delegate = new PSBDelegate();
    view.setModel(&model);
    view.setItemDelegate(delegate);
    view.resizeColumnToContents(0);
    view.resize(500, 300);

    view.show();
    return app.exec();
}

#endif
