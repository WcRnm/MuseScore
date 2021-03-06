//=============================================================================
//  MuseScore
//  Music Composition & Notation
//
//  Copyright (C) 2020 MuseScore BVBA and others
//
//  This program is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License version 2.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software
//  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//=============================================================================
#include "shortcutsinstancemodel.h"

#include <QMainWindow>
#include <QShortcut>

#include "log.h"

using namespace mu::shortcuts;

ShortcutsInstanceModel::ShortcutsInstanceModel(QObject* parent)
    : QObject(parent)
{
}

ShortcutsInstanceModel::~ShortcutsInstanceModel()
{
    m_shortcuts.clear();
}

void ShortcutsInstanceModel::load()
{
    m_shortcuts.clear();

    QWidget* mainWindowWidget = dynamic_cast<QWidget*>(mainWindow()->qMainWindow());
    if (!mainWindowWidget) {
        LOGE() << "Main window widget is not valid";
        return;
    }

    QList<std::string> sequences;
    for (const Shortcut& shortcut: shortcutsRegister()->shortcuts()) {
        //! NOTE There may be several identical shortcuts for different contexts.
        //! We only need a list of unique ones.
        if (sequences.contains(shortcut.sequence)) {
            continue;
        }

        QShortcut* qshortcut = new QShortcut(QString::fromStdString(shortcut.sequence), mainWindowWidget, [this, shortcut]() {
            controller()->activate(shortcut.sequence);
        }, Qt::ApplicationShortcut);

        m_shortcuts.push_back(qshortcut);
        sequences << shortcut.sequence;
    }
}
