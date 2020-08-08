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

#include <KIcon>

#include "improfile.h"

namespace IMChooser
{

IMProfile::IMProfile(KConfigGroup& group)
{
    m_id = group.name();
    m_name = group.readEntry<QString>("name", "");

    QString iconName = group.readEntry<QString>("icon", "");
    m_icon = KIcon(iconName);
    m_command = group.readEntry<QString>("command", "");
    m_argument = group.readEntry<QString>("argument", "");

    KConfigGroup envvar(&group, "envvar");
    m_envVars = envvar.entryMap();
}

const QString& IMProfile::id()
{
    return m_id;
}

const QIcon& IMProfile::icon()
{
    return m_icon;
}

const QString& IMProfile::name()
{
    return m_name;
}

const QString& IMProfile::command()
{
    return m_command;
}

const QString& IMProfile::argument()
{
    return m_argument;
}

const QMap< QString, QString >& IMProfile::envVars()
{
    return m_envVars;
}


}