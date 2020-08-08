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


#include <QFile>
#include <QDir>

#include <KPluginFactory>
#include <KStandardDirs>
#include <KSharedConfig>
#include <KSharedConfigPtr>
#include <KDebug>
#include <KMessageBox>
#include <KIconButton>
#include <KFileDialog>
//kdelibs4support
#include <KGlobal>
#include <KIcon>
#include <KDialog>
#include <KUrl>

#include <QStandardPaths>

#define TRANSLATION_DOMAIN "kcm_imchooser"
#include <klocalizedstring.h>

#include "ui_module.h"
#include "module.h"
#include "profilegenerator.h"
#include "improfilemodel.h"
#include "improfile.h"

K_PLUGIN_FACTORY_DECLARATION(KcmIMChooserFactory);

namespace IMChooser
{
Module::Module(QWidget *parent, const QVariantList &args) :
    KCModule(parent, args),
    ui(new Ui::Module),
    m_model(new IMProfileModel(this)),
    m_listView(0),
    m_profilesConfig(KSharedConfig::openConfig("imchooserprofilerc", KConfig::CascadeConfig)),
    m_config(KSharedConfig::openConfig("imchooserrc", KConfig::SimpleConfig)),
    m_profileEdited(false)
{
    ui->setupUi(this);
    ui->imIconLabel->setPixmap(KIcon("draw-freehand").pixmap(16));
    ui->envIconLabel->setPixmap(KIcon("system-run").pixmap(16));
    ui->newProfileButton->setIcon(KIcon("document-new"));
    ui->editProfileButton->setIcon(KIcon("document-edit"));
    ui->deleteProfileButton->setIcon(KIcon("edit-delete"));
    ui->resetProfileButton->setIcon(KIcon("document-revert"));
    ui->addEnvVarButton->setIcon(KIcon("list-add"));;
    ui->removeEnvVarButton->setIcon(KIcon("list-remove"));
    ui->imBrowseButton->setIcon(KIcon("document-open"));

    if (m_profilesConfig->groupList().isEmpty()) {
        ProfileGenerator::generateProfiles();
        m_profilesConfig->reparseConfiguration();
    }

    ui->profileListView->setModel(m_model);
    m_listView = ui->profileListView;

    connect(ui->newProfileButton, SIGNAL(clicked(bool)), this, SLOT(newProfile()));
    connect(ui->editProfileButton, SIGNAL(clicked(bool)), this, SLOT(editProfile()));
    connect(ui->deleteProfileButton, SIGNAL(clicked(bool)), this, SLOT(deleteProfile()));
    connect(ui->resetProfileButton, SIGNAL(clicked(bool)), this, SLOT(restoreDefaultProfiles()));
    connect(ui->profileListView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
            this, SLOT(currentChanged(QModelIndex, QModelIndex)));
    connect(ui->addEnvVarButton, SIGNAL(clicked(bool)), this, SLOT(addEnvVar()));
    connect(ui->imBrowseButton, SIGNAL(clicked(bool)), this, SLOT(browseIM()));
    connect(ui->removeEnvVarButton, SIGNAL(clicked(bool)), this, SLOT(removeEnvVar()));
    connect(ui->imCommandEdit, SIGNAL(textChanged(QString)), this, SLOT(profileChanged()));
    connect(ui->imArgEdit, SIGNAL(textChanged(QString)), this, SLOT(profileChanged()));
    connect(ui->envvarTableWidget, SIGNAL(cellChanged(int, int)), this, SLOT(profileChanged()));
}

Module::~Module()
{
}

void Module::load()
{
    m_profilesConfig->reparseConfiguration();
    m_model->load(m_profilesConfig);

    KConfigGroup group(m_config, "IMChooser");
    QString defaultIM = group.readEntry<QString>("DefaultIM", "None");
    QModelIndex ind = m_model->searchById(defaultIM);
    m_listView->setCurrentIndex(ind);
    loadProfile();
}

void Module::save()
{
    KConfigGroup group(m_config, "IMChooser");
    QString profile;
    if (!m_listView->currentIndex().isValid()) {
        group.writeEntry<QString>("DefaultIM", "None");
        profile = "None";
    } else {
        profile = m_listView->currentIndex().data(Qt::UserRole).toString();
        group.writeEntry<QString>("DefaultIM", profile);
        saveProfile(profile);
    }

    QDir kdedir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/plasma-workspace");
    if (!kdedir.exists("env"))
        kdedir.mkpath("env");

    QFile envScript(kdedir.path() + "/env/imchooser-env.sh");
    if (!envScript.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    if (profile != "None") {
        KConfigGroup group(m_profilesConfig, profile);
        KConfigGroup envvarGroup(&group, "envvar");
        QMap<QString, QString> envvar = envvarGroup.entryMap();
        QMapIterator<QString, QString> iter(envvar);

        QString t("export %1=\"%2\"\n");
        while (iter.hasNext()) {
            iter.next();
            envScript.write(t.arg(iter.key()).arg(iter.value()).toLocal8Bit());
        }
    }
    envScript.close();

}

void Module::defaults()
{
    restoreDefaultProfiles();
}

void Module::restoreDefaultProfiles()
{
    // Confirm
    int ret = KMessageBox::warningContinueCancel(this, i18n("The KDE Input Method Chooser will now generate a set of default "
              "profiles. Are you sure you want to continue?"), i18n("Restore Default Profiles"));
    if (ret == KMessageBox::Continue) {
        kDebug() << "Restoring defaults.";
        ProfileGenerator::generateProfiles();

        load();
    }
}

void Module::newProfile()
{
    KDialog *dialog = new KDialog(this);
    QWidget *wg = new QWidget();
    KLineEdit *ed = new KLineEdit(wg);
    QLabel *lb = new QLabel(wg);
    QFormLayout *lay = new QFormLayout();
    KIconButton *ibt = new KIconButton(wg);

    ibt->setIconSize(KIconLoader::SizeSmall);

    lb->setText(i18n("Please enter a name for the new profile:"));

    lb->setToolTip(i18n("The name for the new profile"));
    lb->setWhatsThis(i18n("Enter here the name for the profile you are creating"));

    ed->setToolTip(i18n("The name for the new profile"));
    ed->setWhatsThis(i18n("Enter here the name for the profile you are creating"));

    lay->addRow(lb);
    lay->addRow(ibt, ed);

    wg->setLayout(lay);

    dialog->setMainWidget(wg);
    ed->setFocus();

    if (dialog->exec() == KDialog::Accepted) {
        newProfile(ed->text(), ibt->icon());
    }
    delete dialog;
}

void Module::newProfile(const QString &name, const QString &icon)
{
    if (name.isEmpty()) {
        return;
    }
    KConfigGroup group(m_profilesConfig, name);
    group.writeEntry("name", name);
    group.writeEntry("icon", icon);
    group.writeEntry<QString>("command", "");
    group.writeEntry<QString>("argument", "");

    group.sync();

    load();
}

void Module::editProfile()
{
    if (!m_listView->currentIndex().isValid()) {
        return;
    }

    KDialog *dialog = new KDialog(this);
    QWidget *wg = new QWidget();
    KLineEdit *ed = new KLineEdit(wg);
    QLabel *lb = new QLabel(wg);
    QFormLayout *lay = new QFormLayout();
    KIconButton *ibt = new KIconButton(wg);

    ibt->setIconSize(KIconLoader::SizeSmall);

    lb->setText(i18n("Please enter a name for this profile:"));

    lb->setToolTip(i18n("The name for the new profile"));
    lb->setWhatsThis(i18n("Enter here the name for the profile you are creating"));

    ed->setToolTip(i18n("The name for the new profile"));
    ed->setWhatsThis(i18n("Enter here the name for the profile you are creating"));

    KConfigGroup group(m_profilesConfig, m_listView->currentIndex().data(Qt::UserRole).toString());

    ibt->setIcon(group.readEntry("icon"));

    lay->addRow(lb);
    lay->addRow(ibt, ed);

    wg->setLayout(lay);

    dialog->setMainWidget(wg);
    ed->setFocus();

    if (dialog->exec() == KDialog::Accepted) {
        editProfile(m_listView->currentIndex().data(Qt::UserRole).toString(), ed->text(), ibt->icon());
    }

    delete dialog;
}

void Module::editProfile(const QString &id, const QString &name, const QString &icon)
{
    if (id.isEmpty() || !m_profilesConfig->hasGroup(id)) {
        return;
    }

    KConfigGroup group(m_profilesConfig, id);

    group.writeEntry("icon", icon);
    group.writeEntry("name", name);

    group.sync();

    load();
}

void Module::deleteProfile()
{
    if (!m_listView->currentIndex().isValid()) {
        return;
    }

    changed();

    m_profilesConfig->deleteGroup(m_listView->currentIndex().data(Qt::UserRole).toString());
    m_profilesConfig->sync();

    load();
}

void Module::currentChanged(const QModelIndex& current, const QModelIndex& previous)
{
    Q_UNUSED(current)

    if (!m_profileEdited) {
        kDebug() << "Profile has not been edited, switch";
        loadProfile();
    } else {
        if (!previous.isValid()) {
            // Pass by, the profile has probably been deleted
            kDebug() << "No previous profile";
            loadProfile();
            return;
        } else if (!m_profilesConfig.data()->groupList().contains(previous.data(Qt::UserRole).toString())) {
            // Pass by, the profile has probably been deleted
            kDebug() << "No previous profile saved";
            loadProfile();
            return;
        }

        int result = KMessageBox::warningYesNoCancel(this, i18n("The current profile has not been saved.\n"
                     "Do you want to save it?"), i18n("Save Profile"));

        if (result == KMessageBox::Yes) {
            saveProfile(previous.data(Qt::UserRole).toString());
            loadProfile();
        } else if (result == KMessageBox::No) {
            loadProfile();
        } else if (result == KMessageBox::Cancel) {
            disconnect(ui->profileListView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
                       this, SLOT(currentChanged(QModelIndex, QModelIndex)));
            m_listView->setCurrentIndex(previous);
            connect(ui->profileListView->selectionModel(), SIGNAL(currentChanged(QModelIndex, QModelIndex)),
                    this, SLOT(currentChanged(QModelIndex, QModelIndex)));
        }
    }
}

void Module::loadProfile()
{
    kDebug() << "Loading a profile";

    if (!m_listView->currentIndex().isValid()) {
        return;
    }

    QString profileId = m_listView->currentIndex().data(Qt::UserRole).toString();
    if (profileId == "None") {
        ui->editProfileButton->setEnabled(false);
        ui->deleteProfileButton->setEnabled(false);
        ui->imArgEdit->setEnabled(false);
        ui->imCommandEdit->setEnabled(false);
        ui->imBrowseButton->setEnabled(false);
        ui->envvarTableWidget->setEnabled(false);
        ui->addEnvVarButton->setEnabled(false);
        ui->removeEnvVarButton->setEnabled(false);
    } else {
        ui->deleteProfileButton->setEnabled(true);
        ui->editProfileButton->setEnabled(true);
        ui->imArgEdit->setEnabled(true);
        ui->imCommandEdit->setEnabled(true);
        ui->imBrowseButton->setEnabled(true);
        ui->envvarTableWidget->setEnabled(true);
        ui->addEnvVarButton->setEnabled(true);
        ui->removeEnvVarButton->setEnabled(true);
    }

    // Check if the profile is not reserved
    IMProfile *profile = static_cast<IMProfile*>(m_listView->currentIndex().internalPointer());

    ui->imCommandEdit->setText(profile->command());
    ui->imArgEdit->setText(profile->argument());

    ui->envvarTableWidget->clearContents();

    QMapIterator<QString, QString> iter(profile->envVars());

    ui->envvarTableWidget->setRowCount(profile->envVars().size());

    int i = 0;
    while (iter.hasNext()) {
        iter.next();
        ui->envvarTableWidget->setItem(i, 0, new QTableWidgetItem(iter.key()));
        ui->envvarTableWidget->setItem(i, 1, new QTableWidgetItem(iter.value()));
        i ++ ;
    }

    KConfigGroup group(m_config, "IMChooser");
    QString defaultIM = group.readEntry<QString>("DefaultIM", "None");
    if (profile->id() != defaultIM)
        emit changed(true);
    m_profileEdited = false;
}

void Module::saveProfile(QString p)
{
    if (!m_listView->currentIndex().isValid() && p.isEmpty()) {
        kDebug() << "Could not perform a save operation";
        return;
    }

    QString profile;

    if (p.isEmpty()) {
        profile = m_listView->currentIndex().data(Qt::UserRole).toString();
    } else {
        profile = p;
    }

    KConfigGroup group(m_profilesConfig, profile);

    if (!group.isValid()) {
        kDebug() << "Could not perform a save operation, group is not valid!";
        return;
    }

    group.writeEntry<QString>("command", ui->imCommandEdit->text());
    group.writeEntry<QString>("argument", ui->imArgEdit->text());
    group.deleteGroup("envvar");

    KConfigGroup envvar(&group, "envvar");

    for (int i = 0; i < ui->envvarTableWidget->rowCount(); i ++) {
        QString key = ui->envvarTableWidget->item(i, 0)->text();
        QString value = ui->envvarTableWidget->item(i, 1)->text();
        if (!key.isEmpty())
            envvar.writeEntry(key, value);
    }

    envvar.sync();

    group.sync();

    // After saving, reload the config to make sure we'll pick up changes.
    m_profilesConfig->reparseConfiguration();

    m_profileEdited = false;
    emit changed(false);
}

void Module::profileChanged()
{
    m_profileEdited = true;
    changed();
}

void Module::addEnvVar()
{
    KDialog *dialog = new KDialog(this);
    QWidget *wg = new QWidget();
    KLineEdit *keyed = new KLineEdit(wg);
    KLineEdit *valueed = new KLineEdit(wg);
    QLabel *keylb = new QLabel(wg);
    QLabel *valuelb = new QLabel(wg);
    QFormLayout *lay = new QFormLayout();

    keylb->setText(i18n("Name:"));
    valuelb->setText(i18n("Value:"));

    lay->addRow(keylb, keyed);
    lay->addRow(valuelb, valueed);

    wg->setLayout(lay);

    dialog->setMainWidget(wg);
    keyed->setFocus();

    if (dialog->exec() == KDialog::Accepted) {
        addEnvVar(keyed->text(), valueed->text());
    }
    delete dialog;
}

void Module::removeEnvVar()
{
    ui->envvarTableWidget->removeRow(ui->envvarTableWidget->currentRow());

    profileChanged();
}

void Module::addEnvVar(const QString& name, const QString& value)
{
    if (name.isEmpty())
        return;

    ui->envvarTableWidget->insertRow(ui->envvarTableWidget->rowCount());
    ui->envvarTableWidget->setItem(ui->envvarTableWidget->rowCount() - 1, 0, new QTableWidgetItem(name));
    ui->envvarTableWidget->setItem(ui->envvarTableWidget->rowCount() - 1, 1, new QTableWidgetItem(value));

    profileChanged();
}

void Module::browseIM()
{
    QString imExecuable = KFileDialog::getOpenFileName(KUrl("/usr/bin"));
    if (!imExecuable.isEmpty())
        ui->imCommandEdit->setText(imExecuable);
}

}
