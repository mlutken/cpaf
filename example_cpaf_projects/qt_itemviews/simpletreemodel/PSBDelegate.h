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

#ifndef PSBDELEGATE_H
#define PSBDELEGATE_H

#include <QStyledItemDelegate>
#include <QFont>
#include <QColor>

//! [0]
class PSBDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    PSBDelegate(QWidget *parent = 0);

    void setFont( const QString& name, int base_size);
    void setImageSize(int size_in_pixels);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

private:
    enum RenderType { eRenderPatient, eRenderYear, eRenderYearAndStudy, eRenderStudy, eRenderSeries };

    QSize renderTypeToSizeHint( RenderType rt) const;
    void paintPatient(QPainter *painter, const QStyleOptionViewItem &option, const QRect& rect) const;
    void paintYear(QPainter *painter, const QStyleOptionViewItem &option, const QRect& rect) const;
    void drawSeriesBackground(QPainter *painter, const QRect& rect) const;
    void paintYearAndStudy(QPainter *painter, const QStyleOptionViewItem &option, const QRect& rect) const;
    void paintStudy(QPainter *painter, const QStyleOptionViewItem &option, const QRect& rect) const;
    void paintStudyCompact(QPainter *painter, const QStyleOptionViewItem &option, const QRect& rect) const;
    void paintStudyNormal(QPainter *painter, const QStyleOptionViewItem &option, const QRect& rect) const;
    void paintSeriesSeries(QPainter *painter, const QStyleOptionViewItem &option, const QRect& rect) const;

    void drawSelected(QPainter *painter, const QRect& rect, bool selected) const;
    int getWidthHint() const;
    RenderType modelIndexToRenderType(const QModelIndex &index) const;

    int m_image_size_in_pixels;
    QFont m_fontPatient;
    QFont m_fontYear;
    QFont m_fontHeadline1;
    QFont m_fontHeadline2;
    QFont m_fontHeadline3;
    QColor m_colorSelected;

};
//! [0]

#endif //PSBDELEGATE_H

