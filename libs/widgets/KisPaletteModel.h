/*
 *  Copyright (c) 2013 Sven Langkamp <sven.langkamp@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */


#ifndef KIS_PALETTEMODEL_H
#define KIS_PALETTEMODEL_H

#include <QPointer>
#include <QModelIndex>

#include <KoColorDisplayRendererInterface.h>

#include "kritawidgets_export.h"
#include <KoColorSet.h>
#include <QScopedPointer>

class KoColorSet;

/**
 * @brief The KisPaletteModel class
 * This, together with kis_palette_view and kis_palette_delegate forms a mvc way to access kocolorsets.
 */
class KRITAWIDGETS_EXPORT KisPaletteModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    KisPaletteModel(QObject* parent = 0);
    ~KisPaletteModel() override;

    enum AdditionalRoles {
        IsHeaderRole       = Qt::UserRole + 1,
        RetrieveEntryRole  = Qt::UserRole + 3,
        CheckSlotRole = Qt::UserRole + 4
    };

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    /**
     * @brief setData
     * setData is not used as KoColor is not a QVariant
     * use setEntry, addEntry and removeEntry instead
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
     */

    /**
     * @brief addEntry
     * proper function to handle adding entries.
     * @return whether successful.
     */
    bool addEntry(KisSwatch entry, QString groupName=QString());

    void setEntry(const KisSwatch &entry, const QModelIndex &index);

    /**
     * @brief removeEntry
     * proper function to remove the colorsetentry at the given index.
     * The consolidtes both removeentry and removegroup.
     * @param keepColors: This bool determines whether, when deleting a group,
     * the colors should be added to the default group. This is usually desirable,
     * so hence the default is true.
     * @return if successful
     */
    bool removeEntry(QModelIndex index, bool keepColors=true);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    /**
     * @brief index
     * @param row
     * @param column
     * @param parent
     * @return the index of for the data at row, column
     * if the data is a color entry, the internal pointer points to the group
     * the entry belongs to, and the row and column are row number and column
     * number inside the group.
     * if the data is a group, the row number and group number is Q_INFINIFY,
     * and the internal pointer also points to the group
     */
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

    void setColorSet(KoColorSet* colorSet);
    KoColorSet* colorSet() const;

    /**
     * Installs a display renderer object for a palette that will
     * convert the KoColor to the displayable QColor. Default is the
     * dumb renderer.
     */
    void setDisplayRenderer(const KoColorDisplayRendererInterface *displayRenderer);

    /**
     * @brief indexFromId
     * convenience function to get the tableindex from the global palette color.
     * used by lazybrush.
     * @param i
     * @return index in table.
     */
    QModelIndex indexFromId(int i) const;
    /**
     * @brief idFromIndex
     * convenience function to get the global colorset entry id from the table index.
     * If you just want to use this to get the kocolorsetentry, use colorsetEntryFromIndex instead.
     * @param index
     * @return
     */
    int idFromIndex(const QModelIndex &index) const;

    /**
     * @brief colorSetEntryFromIndex
     * This gives the colorset entry for the given table model index.
     * @param index the QModelIndex
     * @return the kocolorsetentry
     */
    KisSwatch colorSetEntryFromIndex(const QModelIndex &index) const;

    /**
     * @brief dropMimeData
     * This is an overridden function that handles dropped mimedata.
     * right now only colorsetentries and colorsetgroups are handled.
     * @return
     */
    bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent) override;
    /**
     * @brief mimeData
     * gives the mimedata for a kocolorsetentry or a kocolorsetgroup.
     * @param indexes
     * @return the mimedata for the given indices
     */
    QMimeData *mimeData(const QModelIndexList &indexes) const override;

    QStringList mimeTypes() const override;

    Qt::DropActions supportedDropActions() const override;

public Q_SLOTS:

private Q_SLOTS:
    void slotDisplayConfigurationChanged();

private:
    QModelIndex getLastEntryIndex();

private:
    QPointer<KoColorSet> m_colorSet;
    QPointer<const KoColorDisplayRendererInterface> m_displayRenderer;
};

#endif