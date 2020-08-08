/***************************************************************************
 *   Copyright (C) 2010~2011 by CSSlayer                                   *
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

#include <KSharedConfig>
#include <KConfigGroup>
#include <KLocalizedString>

#include "module.h"
#include "profilegenerator.h"

namespace IMChooser
{

void ProfileGenerator::generateProfiles()
{
    // Ok, let's get our config file.
    KSharedConfigPtr profilesConfig = KSharedConfig::openConfig("imchooserprofilerc", KConfig::CascadeConfig);

    foreach(const QString & group, profilesConfig->groupList()) {
        profilesConfig->deleteGroup(group);
    }

    generateDummyProfile(profilesConfig);
    generateFcitxProfile(profilesConfig);
    generateIBusProfile(profilesConfig);
    generateScimProfile(profilesConfig);
}

void ProfileGenerator::generateDummyProfile(KSharedConfigPtr& profilesConfig)
{
    KConfigGroup none(profilesConfig, "None");
    none.writeEntry<QString>("name", i18n("None"));
    none.writeEntry<QString>("icon", "system-run");
    none.writeEntry<QString>("command", "");
    none.writeEntry<QString>("argument", "");
}

void ProfileGenerator::generateFcitxProfile(KSharedConfigPtr& profilesConfig)
{
    KConfigGroup fcitx(profilesConfig, "Fcitx");
    fcitx.writeEntry<QString>("name", i18n("Fcitx"));
    fcitx.writeEntry<QString>("icon", "fcitx");
    fcitx.writeEntry<QString>("command", "fcitx");
    fcitx.writeEntry<QString>("argument", "");

    KConfigGroup envvar(&fcitx, "envvar");
    envvar.writeEntry("XMODIFIERS", "@im=fcitx");
    envvar.writeEntry("GTK_IM_MODULE", "fcitx");
    envvar.writeEntry("QT_IM_MODULE", "fcitx");
}


void ProfileGenerator::generateIBusProfile(KSharedConfigPtr& profilesConfig)
{
    KConfigGroup ibus(profilesConfig, "IBus");
    ibus.writeEntry<QString>("name", i18n("IBus"));
    ibus.writeEntry<QString>("icon", "ibus");
    ibus.writeEntry<QString>("command", "ibus-daemon");
    ibus.writeEntry<QString>("argument", "-x");

    KConfigGroup envvar(&ibus, "envvar");
    envvar.writeEntry("XMODIFIERS", "@im=ibus");
    envvar.writeEntry("GTK_IM_MODULE", "ibus");
    envvar.writeEntry("QT_IM_MODULE", "ibus");
}


void ProfileGenerator::generateScimProfile(KSharedConfigPtr& profilesConfig)
{
    KConfigGroup scim(profilesConfig, "Scim");
    scim.writeEntry<QString>("name", i18n("Scim"));
    scim.writeEntry<QString>("icon", "scim");
    scim.writeEntry<QString>("command", "scim");
    scim.writeEntry<QString>("argument", "");

    KConfigGroup envvar(&scim, "envvar");
    envvar.writeEntry("XMODIFIERS", "@im=scim");
    envvar.writeEntry("GTK_IM_MODULE", "scim");
    envvar.writeEntry("QT_IM_MODULE", "scim");
}

}