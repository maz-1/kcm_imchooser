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


#ifndef IMPROFILEMODEL_H
#define IMPROFILEMODEL_H

#include <QList>
#include <QAbstractItemModel>
#include <KSharedConfig>

namespace IMChooser
{

class IMProfile;

class IMProfileModel : public QAbstractListModel
{
    Q_OBJECT
public:
    IMProfileModel(QObject* parent = 0);
    virtual ~IMProfileModel();
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    void load(const KSharedConfigPtr& config);
    QModelIndex searchById(const QString& id);
private:
    QList<IMProfile*> m_profiles;
};

}

#endif