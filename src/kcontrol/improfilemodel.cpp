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

#include <KDebug>

#include "improfilemodel.h"
#include "improfile.h"

namespace IMChooser
{
IMProfileModel::IMProfileModel(QObject* parent): QAbstractListModel(parent)
{

}

IMProfileModel::~IMProfileModel()
{
    Q_FOREACH(IMProfile * profile, m_profiles) {
        delete profile;
    }
}

int IMProfileModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_profiles.size();
}

QVariant IMProfileModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || !index.internalPointer()) {
        return QVariant();
    }

    IMProfile *profile = static_cast<IMProfile*>(index.internalPointer());

    if (role == Qt::DecorationRole)
        return profile->icon();
    else if (role == Qt::DisplayRole)
        return profile->name();
    else if (role == Qt::UserRole)
        return profile->id();


    return QVariant();
}

QModelIndex IMProfileModel::index(int row, int column, const QModelIndex& parent) const
{
    Q_UNUSED(parent);

    return createIndex(row, column, (row < m_profiles.count()) ? (void*) m_profiles.at(row) : 0);
}

void IMProfileModel::load(const KSharedConfigPtr& config)
{
    beginRemoveRows(QModelIndex(), 0, m_profiles.size());
    Q_FOREACH(IMProfile * profile, m_profiles) {
        delete profile;
    }
    m_profiles.clear();
    endRemoveRows();

    Q_FOREACH(const QString & groupName, config->groupList()) {
        KConfigGroup group(config, groupName);
        IMProfile* profile = new IMProfile(group);
        beginInsertRows(QModelIndex(), m_profiles.count(), m_profiles.count());
        m_profiles.append(profile);
        endInsertRows();
    }
}

QModelIndex IMProfileModel::searchById(const QString& id)
{
    int i = 0;
    for (i = 0; i < m_profiles.size(); i ++) {
        if (m_profiles[i]->id() == id)
            return index(i, 0);
    }

    for (i = 0; i < m_profiles.size(); i ++) {
        if (m_profiles[i]->id() == "None")
            return index(i, 0);
    }

    return QModelIndex();
}

}