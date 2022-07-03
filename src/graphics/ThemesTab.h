#pragma once

#include "../logics/ThemeRequest.h"

#include <QWidget>

class ThemesTab : public QWidget {
    Q_OBJECT

    public:
        ThemesTab(QWidget* parent = nullptr);

    signals:
        void listRequested(
            const ThemeRequest& fields,
            const QString& themeNameKeyword,
            int packageId,
            Qt::CheckState inWishlist,
            Qt::CheckState isLearned);
};
