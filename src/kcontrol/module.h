/***************************************************************************
 *   Copyright (C) 2011~2011 by CSSlayer                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.              *
 ***************************************************************************/

#ifndef MODULE_H
#define MODULE_H

#include <KCModule>
#include <KSharedConfig>
#include <QModelIndex>

namespace Ui
{
class Module;
}

class QListView;

namespace IMChooser
{

class IMProfileModel;

class Module : public KCModule
{
    Q_OBJECT

public:
    /**
    * Constructor.
    *
    * @param parent Parent widget of the module
    * @param args Arguments for the module
    */
    Module(QWidget *parent, const QVariantList &args = QVariantList());

    /**
    * Destructor.
    */
    ~Module();

    /**
    * Overloading the KCModule load() function.
    */
    void load();

    /**
    * Overloading the KCModule save() function.
    */
    void save();

    /**
    * Overloading the KCModule defaults() function.
    *
    * Setting all radio buttons to unchecked, which indicates for save() to
    * restore to system default (i.e. remove the script file).
    */
    void defaults();
    void saveProfile(const QString name);
    void loadProfile();

protected slots:
    void deleteProfile();
    void newProfile();
    void editProfile();
    void restoreDefaultProfiles();
    void currentChanged(const QModelIndex &current, const QModelIndex &previous);
    void profileChanged();
    void addEnvVar();
    void removeEnvVar();
    void browseIM();

protected:
    void newProfile(const QString &name, const QString &icon);
    void addEnvVar(const QString &name, const QString &value);
    void editProfile(const QString &id, const QString &name, const QString &icon);

private:
    /**
    * UI
    */
    Ui::Module *ui;
    IMProfileModel* m_model;
    QListView* m_listView;
    KSharedConfig::Ptr m_profilesConfig;
    KSharedConfig::Ptr m_config;
    bool m_profileEdited;
};

}

#endif // MODULE_H
