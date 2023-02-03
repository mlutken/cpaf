#ifndef CAMUTILITY_SYNONYMSEDITORDIALOG_H
#define CAMUTILITY_SYNONYMSEDITORDIALOG_H

#include <QDialog>
#include <camutility/config.h>

namespace camutility {

class SynonymsEditor;

class CAMUTILITY_EXPORT SynonymsEditorDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SynonymsEditorDialog(QWidget *parent = 0);

Q_SIGNALS:

public Q_SLOTS:

private:
    SynonymsEditor *m_synonyms_editor;

};

} // END namespace camutility

#endif // CAMUTILITY_SYNONYMSEDITORDIALOG_H
