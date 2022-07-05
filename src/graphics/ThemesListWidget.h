#pragma once

#include "../datatypes/Theme.h"
#include "../datatypes/ThemeRequest.h"

#include <QListWidget>
#include <QVector>

class ThemesListWidget : public QListWidget {
    Q_OBJECT

    public:
        ThemesListWidget(QWidget* parent = nullptr);

    signals:
        void listRequested(
            const ThemeRequest& fields,
            const QString& themeNameKeyword,
            int packageId,
            Qt::CheckState inWishlist,
            Qt::CheckState isLearned);

    private slots:
        void onListGot(const QVector<Theme>& themes);
        void showContextMenu(const QPoint& pos);
};
