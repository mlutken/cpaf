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

#include <QtWidgets>
#include <QRect>

#include "stardelegate.h"
#include "stareditor.h"
#include "starrating.h"


StarDelegate::StarDelegate(QWidget *parent)
    : QStyledItemDelegate(parent)
{
    setFont("Arial", 12); // TODO: From settings or similar
    setImageSize(100);    // TODO: From settings or similar
}

void StarDelegate::setFont(const QString &name, int base_size)
{
    m_fontYear = QFont(name, base_size+3, 0, false);
    m_fontHeadline1 = QFont(name, base_size+2, 0, false);
    m_fontHeadline2 = QFont(name, base_size, 0, false);
    m_fontHeadline3 = QFont(name, base_size-2, 0, false);
}

void StarDelegate::setImageSize(int size_in_pixels)
{
    m_image_size_in_pixels = size_in_pixels;
}


//! [0]
void StarDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                         const QModelIndex &index) const
{
    const QRect& rect = option.rect;
    if ( index.column() == 0 ) {
        switch ( index.row() ) {
        case 0 : paintSeriesHeadLine(painter, option, rect); break;
        case 1 : paintYear(painter, option, rect); break;
        case 2 : paintSeriesHeadLine(painter, option, rect); break;
        case 3 : paintSeriesSeries(painter, option, rect); break;
        default:
            paintSeriesSeries(painter, option, rect); break;
            //QStyledItemDelegate::paint(painter, option, index);

        }
    }
    else {
        QStyledItemDelegate::paint(painter, option, index);
    }

//    if (index.data().canConvert<StarRating>()) {
//        StarRating starRating = qvariant_cast<StarRating>(index.data());

//        if (option.state & QStyle::State_Selected)
//            painter->fillRect(option.rect, option.palette.highlight());

//        starRating.paintYear(painter, option.rect, option.palette,
//                         StarRating::ReadOnly);
//    } else {
//        QStyledItemDelegate::paint(painter, option, index);
//    }
}

QSize StarDelegate::sizeHint(const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    const int series_series_height = m_fontHeadline1.pointSize()*2+10;
    QSize defaultSize = QSize(300,20);
    switch (index.row() ) {
    case 0 : return QSize(defaultSize.width(), series_series_height);
    case 1 : return QSize(defaultSize.width(), m_fontYear.pointSize()+4); // Year
    case 2 : return QSize(defaultSize.width(), series_series_height);
    case 3 : return QSize(defaultSize.width(), m_image_size_in_pixels);
    default:
        return QSize(defaultSize.width(), m_image_size_in_pixels);
        //return defaultSize;

    }

    if (index.data().canConvert<StarRating>()) {
        StarRating starRating = qvariant_cast<StarRating>(index.data());
        return starRating.sizeHint();
    } else {
        return QStyledItemDelegate::sizeHint(option, index);
    }
}

QWidget *StarDelegate::createEditor(QWidget *parent,
                                    const QStyleOptionViewItem &option,
                                    const QModelIndex &index) const

{
    if (index.data().canConvert<StarRating>()) {
        StarEditor *editor = new StarEditor(parent);
        connect(editor, SIGNAL(editingFinished()),
                this, SLOT(commitAndCloseEditor()));
        return editor;
    } else {
        return QStyledItemDelegate::createEditor(parent, option, index);
    }
}

void StarDelegate::setEditorData(QWidget *editor,
                                 const QModelIndex &index) const
{
    if (index.data().canConvert<StarRating>()) {
        StarRating starRating = qvariant_cast<StarRating>(index.data());
        StarEditor *starEditor = qobject_cast<StarEditor *>(editor);
        starEditor->setStarRating(starRating);
    } else {
        QStyledItemDelegate::setEditorData(editor, index);
    }
}

void StarDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                const QModelIndex &index) const
{
    if (index.data().canConvert<StarRating>()) {
        StarEditor *starEditor = qobject_cast<StarEditor *>(editor);
        model->setData(index, QVariant::fromValue(starEditor->starRating()));
    } else {
        QStyledItemDelegate::setModelData(editor, model, index);
    }
}

void StarDelegate::commitAndCloseEditor()
{
    StarEditor *editor = qobject_cast<StarEditor *>(sender());
    emit commitData(editor);
    emit closeEditor(editor);
}


void StarDelegate::paintYear(QPainter *painter, const QStyleOptionViewItem &/*option*/, const QRect& rect) const
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);

    QLinearGradient gradient(rect.topLeft(), rect.bottomRight());
    gradient.setColorAt(0, QColor(177,177,177));
    gradient.setColorAt(1, QColor(237,237,237));
    QBrush brush(gradient);
    //QBrush brush( QColor(207,207,207), Qt::SolidPattern );

    painter->fillRect(rect, brush );
    painter->setPen(QColor(157,157,157));
    painter->drawLine(rect.x(), rect.y()+0, rect.width(), rect.y()+0);
    painter->drawLine(rect.x(), rect.bottomLeft().y()-0, rect.width(), rect.bottomLeft().y()-0);

    painter->setPen(Qt::black);
    QFont font = m_fontYear;
    font.setBold(true);
    painter->setFont(font);
    painter->drawText(rect.bottomLeft() + QPointF(0, -3), "2012");

    painter->restore();
}

void StarDelegate::drawSeriesBackground(QPainter *painter, const QRect &rect, bool /*selected*/) const
{
    ////painter->save();
    QLinearGradient gradient(rect.topLeft(), rect.bottomRight());
    gradient.setColorAt(0, QColor(0,0,0));
    gradient.setColorAt(1, QColor(38,38,38));
    QBrush brush(gradient);
    //QBrush brush( QColor(207,207,207), Qt::SolidPattern );

    QPen pen (QColor(207,207,207));
    pen.setWidth(2);
    painter->setPen(pen);
    painter->fillRect(rect, brush);
    painter->drawRect(rect.x()+1, rect.y()+1, rect.width()-1, rect.height()-1);
    ////painter->restore();
}


void StarDelegate::paintSeriesHeadLine(QPainter *painter, const QStyleOptionViewItem &option, const QRect& rect) const
{
    painter->save();
    if ( rect.height() > (m_fontHeadline1.pointSize() +m_fontHeadline3.pointSize())+10) {
        paintSeriesHeadLineNormal(painter, option, rect);
    }
    else {
        paintSeriesHeadLineCompact(painter, option, rect);
    }
    painter->restore();
}

void StarDelegate::paintSeriesHeadLineCompact(QPainter *painter, const QStyleOptionViewItem &/*option*/, const QRect& rect) const
{
    painter->setRenderHint(QPainter::Antialiasing, true);
    drawSeriesBackground(painter, rect, false);
    painter->setPen(QColor(227,227,227));
    painter->setFont(m_fontHeadline1);
    painter->drawText(rect.topLeft() + QPointF(3, m_fontHeadline1.pointSize()+3), "17/12 - MRI, BRAIN W/WO...");
}

void StarDelegate::paintSeriesHeadLineNormal(QPainter *painter, const QStyleOptionViewItem &/*option*/, const QRect& rect) const
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);
    drawSeriesBackground(painter, rect, false);

    painter->setPen(QColor(227,227,227));
    painter->setFont(m_fontHeadline1);
    painter->drawText(rect.topLeft() + QPointF(3, m_fontHeadline1.pointSize()+3), "17/12 - Acc: MI.1135-4567-47");
    painter->setPen(QColor(207,207,207));
    if ( rect.height() > (m_fontHeadline1.pointSize() +m_fontHeadline2.pointSize())+10) {
        painter->setFont(m_fontHeadline2);
        painter->drawText(rect.bottomLeft() + QPointF(3, -3), "MRI, BRAIN W/WO...");
    }
    painter->restore();
}

void StarDelegate::paintSeriesSeries(QPainter *painter, const QStyleOptionViewItem &/*option*/, const QRect& rect) const
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing, true);
    drawSeriesBackground(painter, rect, false);

    QPixmap thumnail_original("thumbnail1.png");
    const int img_size = std::min<int>( rect.height()-5, rect.width()/2 );
    QPixmap thumnail_scaled = thumnail_original.scaled(QSize(img_size, img_size), Qt::KeepAspectRatio) ;
    painter->drawImage(rect.topLeft() + QPoint(2,2), thumnail_scaled.toImage());

    painter->setPen(QColor(227,227,227));
    painter->setFont(m_fontHeadline2);

    const int delta_y = img_size/7.5;
    const int text_left = rect.topLeft().x() + img_size+m_fontHeadline2.pointSize()/2;
    painter->drawText(text_left, rect.topLeft().y()+delta_y+m_fontHeadline2.pointSize()+2, "ARTERIAL 5.0");
    painter->setPen(QColor(207,207,207));

    if ( rect.height() > m_fontHeadline2.pointSize()*2+10) {
        painter->drawText(text_left, rect.topLeft().y() + img_size -delta_y, "MR");
        painter->drawText(rect.bottomRight().x() -4*m_fontHeadline2.pointSize(), rect.topLeft().y() + img_size -delta_y, "2345");
    }
    painter->restore();
}
