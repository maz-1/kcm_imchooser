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

#include <QProcess>

#include <KPluginFactory>
#include <KPluginLoader>
#include <KSharedConfig>
#include <KConfigGroup>

#include "imchooserstarter.h"

K_PLUGIN_FACTORY(IMChooserFactory, registerPlugin<IMChooserStarter>();)
K_EXPORT_PLUGIN(IMChooserFactory("kded_imchooserstarter"))

#include "imchooserstarter.moc"

IMChooserStarter::IMChooserStarter(QObject *parent, const QList<QVariant> &)
    : KDEDModule(parent)
{
    KSharedConfig::Ptr profilesConfig = KSharedConfig::openConfig("imchooserprofilerc", KConfig::CascadeConfig);
    KSharedConfig::Ptr config = KSharedConfig::openConfig("imchooserrc", KConfig::SimpleConfig);
    KConfigGroup group(config, "IMChooser");
    QString defaultIM = group.readEntry<QString>("DefaultIM", "None");

    if (defaultIM == "None")
        return;

    KConfigGroup imgroup(profilesConfig, defaultIM);

    QString command = imgroup.readEntry<QString>("command", "");
    QString argument = imgroup.readEntry<QString>("argument", "");
    if (command.isEmpty())
        return;

    QStringList arg;
    arg << "-c" << QString("%1 %2").arg(command).arg(argument);
    QProcess::startDetached("/bin/sh", arg);
}

IMChooserStarter::~IMChooserStarter()
{

}
