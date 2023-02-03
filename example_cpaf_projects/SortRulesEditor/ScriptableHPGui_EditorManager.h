#ifndef SCRIPTABLEHPGUI_EDITORMANAGER_H
#define SCRIPTABLEHPGUI_EDITORMANAGER_H

#include <QObject>

class ScriptHolder;

/** Manager for the scriptable hanging protocols GUI editor.
Has a pointer to each of the individual "editors" (tabs/steps in pipeline).
It has a ScriptHolder indside and takes care of loading/persisting the scripts
from/into the editors.*/
class ScriptableHPGui_EditorManager : public QObject
{
    Q_OBJECT
public:
    explicit ScriptableHPGui_EditorManager(QObject *parent = 0);
    virtual ~ScriptableHPGui_EditorManager() override;

signals:

public slots:
};

#endif // SCRIPTABLEHPGUI_EDITORMANAGER_H
