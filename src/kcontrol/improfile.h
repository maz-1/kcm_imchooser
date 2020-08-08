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

#ifndef IMPROFILE_H
#define IMPROFILE_H

#include <QIcon>

#include <KConfigGroup>

namespace IMChooser
{

class IMProfile
{
public:
    IMProfile(KConfigGroup& group);
    const QString& id();
    const QIcon& icon();
    const QString& name();
    const QString& command();
    const QString& argument();
    const QMap<QString, QString>& envVars();
private:
    QString m_id;
    QString m_name;
    QIcon m_icon;
    QString m_command;
    QString m_argument;
    QMap<QString, QString> m_envVars;
};

}

#endif