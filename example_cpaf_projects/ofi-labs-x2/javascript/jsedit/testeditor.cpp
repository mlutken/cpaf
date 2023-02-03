/*
    This file is part of the Ofi Labs X2 project.

    Copyright (C) 2010 Ariya Hidayat <ariya.hidayat@gmail.com>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of
    the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtGui>

#include "jsedit.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    const char *fileName = (argc > 1) ? argv[1] : ":/prototype.js";
    QFile file(fileName);
    file.open(QFile::ReadOnly);
    QString contents = file.readAll();
    file.close();
    contents.replace('\t', "  ");

    JavascriptEdit editor;
    editor.setWindowTitle(QFileInfo(fileName).fileName());
    editor.setFrameShape(JavascriptEdit::NoFrame);
    editor.setWordWrapMode(QTextOption::NoWrap);
    editor.setTabStopWidth(4);
    editor.resize(QApplication::desktop()->availableGeometry().size() / 2);

    QStringList keywords = editor.keywords();
    keywords << "const";
    keywords << "let";
    editor.setKeywords(keywords);

    // dark color scheme
    editor.setColor(JavascriptEdit::Background,    QColor("#0C152B"));
    editor.setColor(JavascriptEdit::Normal,        QColor("#FFFFFF"));
    editor.setColor(JavascriptEdit::Comment,       QColor("#666666"));
    editor.setColor(JavascriptEdit::Number,        QColor("#DBF76C"));
    editor.setColor(JavascriptEdit::String,        QColor("#5ED363"));
    editor.setColor(JavascriptEdit::Operator,      QColor("#FF7729"));
    editor.setColor(JavascriptEdit::Identifier,    QColor("#FFFFFF"));
    editor.setColor(JavascriptEdit::Keyword,       QColor("#FDE15D"));
    editor.setColor(JavascriptEdit::BuiltIn,       QColor("#9CB6D4"));
    editor.setColor(JavascriptEdit::Cursor,        QColor("#1E346B"));
    editor.setColor(JavascriptEdit::Marker,        QColor("#DBF76C"));
    editor.setColor(JavascriptEdit::BracketMatch,  QColor("#1AB0A6"));
    editor.setColor(JavascriptEdit::BracketError,  QColor("#A82224"));
    editor.setColor(JavascriptEdit::FoldIndicator, QColor("#555555"));

    editor.setPlainText(contents);
    editor.show();

    return app.exec();
}
